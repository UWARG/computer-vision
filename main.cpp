/*
    This file is part of WARG's computer-vision

    Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <boost/lexical_cast.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <functional>
#include <boost/program_options.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <queue>
#include <chrono>
#include <iostream>
#include <fstream>
#include "frame.h"
#include "target_identifier.h"
#include "target_analyzer.h"
#include "imgimport.h"
#include "importer.h"
#include "decklink_import.h"
#include "pictureimport.h"
#include "metadata_input.h"
#include "metadata_reader.h"
#include "target.h"
#include "camera.h"
#include "video_import.h"
#include "pixel_object.h"

using namespace std;
using namespace boost;
using namespace cv;
namespace logging = boost::log;
namespace po = boost::program_options;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

Frame* next_image();
int handle_args(int argc, char** argv);
void handle_input();
void handle_state_change();
queue<Target*> out_buffer;
queue<Frame*> intermediate_buffer;

boost::asio::io_service ioService;
boost::thread_group threadpool;

vector<string> file_names;
int workers = 0;
string outputDir = "./";
bool intermediate = false;
int processors;
bool videoFeed = false;

// Processing module classes
Importer importer;
TargetIdentifier identifier;
TargetAnalyzer * analyzer = NULL;
MetadataInput *logReader = new MetadataInput();

double aveFrameTime = 1000;
int frameCount = 0;

double fisheyeMatrix[] = {
    2.4052826789763981e+03, 0, 2000,
    0, 2.4052826789763981e+03, 1500,
    0, 0, 1
};

double fisheyeDistortion[] = {
    -0.392, 0.146, 0, 0, -0.023
};

double newFisheyeMatrix[] = {
    2000, 0, 2000,
    0, 2000, 1500,
    0, 0, 1
};

double rectMatrix[] = {
    2410, 0, 960,
    0, 2410, 540,
    0, 0, 1
};

double rectDistortion[] = {
    0, 0, 0, 0, 0
};

Camera goProFisheye(
    Size(4000, 3000),
    Size2d(120, 90),
    Mat(
        Size(3, 3),
        CV_64F,
        fisheyeMatrix
    ),
    Mat(
        Size(5, 1),
        CV_64F,
        fisheyeDistortion
    ),
    Mat(
        Size(3, 3),
        CV_64F,
        newFisheyeMatrix
    )
);

Camera goProRect(
    Size(1920, 1080),
    Size2d(90, 67.5),
    Mat(
        Size(3, 3),
        CV_64F,
        rectMatrix
    ),
    Mat(
        Size(5, 1),
        CV_64F,
        rectDistortion
    ),
    Mat(
        Size(3, 3),
        CV_64F,
        rectMatrix
    )
);

struct State {
    bool hasImageSource;
    bool hasMetadataSource;
    bool readingImages;
} currentState;

void worker(Frame* f) {
    auto start = std::chrono::steady_clock::now();
    workers++;
    assert(!f->get_img().empty());
    identifier.process_frame(f);
    if ((intermediate && f->get_objects().size() > 0) || videoFeed) {
        intermediate_buffer.push(f);
    }
    
    //Analyze the image after it is identified
    analyzer = TargetAnalyzer::getInstance();
    for (int i = 0; i < poSize; i++){
        analyzer->analyze_pixelobject(f->get_objects()[i]);
    }

    workers--;
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    aveFrameTime = (std::chrono::duration <double, std::milli>(diff).count() + aveFrameTime*frameCount)/++frameCount;
}

void assign_workers() {
    Frame* current;
    while (currentState.readingImages) {
        if ((current = importer.next_frame()) != NULL) {

            // spawn worker to process image;
            BOOST_LOG_TRIVIAL(trace) << "Spawning worker...";
            ioService.post(boost::bind(worker, current));
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(aveFrameTime/processors));
    }
}

void output() {
    filebuf fb;
    while (currentState.readingImages || out_buffer.size() > 0 || intermediate_buffer.size() > 0 || workers > 0) {
        if (out_buffer.size() > 0) {
            fb.open("out.txt", ios::app);
            ostream out(&fb);

            out << out_buffer.front();
            // Output
            out_buffer.pop();
            fb.close();
        }
        if (intermediate_buffer.size() > 0) {
            Frame * f = intermediate_buffer.front();
            if (f->get_objects().size() > 0) {
                f->save(outputDir);
            }
            if (videoFeed) {
                Mat tmp = f->get_img().clone();
                for (PixelObject *p : f->get_objects()) {
                    vector<vector<Point> > contours;
                    contours.push_back(p->get_contour());
                    Scalar color(rand()&255, rand()&255, rand()&255);
                    drawContours(tmp, contours, 0, color, FILLED, 8);
                }
                BOOST_LOG_TRIVIAL(debug) << "Displaying image: " << f->get_id();
                Mat resized;
                resize(tmp, resized, Size(((double)tmp.cols / tmp.rows) * 600, 600));
                imshow("Live Feed", resized);
                waitKey(1);
            }
            intermediate_buffer.pop();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void init() {
    /* init boost log
     * 1. Add common attributes
     * 2. set log filter to trace
     */
    boost::log::add_common_attributes();
    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::trace
    );

    /* log formatter:
     * [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message
     */
    auto fmtTimeStamp = boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
    auto fmtThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
    auto fmtSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity");
    auto fmtScope = boost::log::expressions::format_named_scope("Scope",
        boost::log::keywords::format = "%n(%f:%l)",
        boost::log::keywords::iteration = boost::log::expressions::reverse,
        boost::log::keywords::depth = 2);
    boost::log::formatter logFmt =
        boost::log::expressions::format("[%1%] (%2%) [%3%] [%4%] %5%")
        % fmtTimeStamp % fmtThreadId % fmtSeverity % fmtScope
        % boost::log::expressions::smessage;

    /* console sink */
    auto consoleSink = boost::log::add_console_log(std::clog);
    consoleSink->set_formatter(logFmt);
    consoleSink->set_filter(logging::trivial::severity >= logging::trivial::error);

    /* fs sink */
    auto fsSink = boost::log::add_file_log(
        boost::log::keywords::file_name = "warg-cv_%Y-%m-%d_%H-%M-%S.%N.log",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::min_free_space = 30 * 1024 * 1024,
        boost::log::keywords::open_mode = std::ios_base::app);
    fsSink->set_formatter(logFmt);
    fsSink->locked_backend()->auto_flush(true);
}

int main(int argc, char** argv) {
    init();
    currentState.hasImageSource = currentState.hasMetadataSource = currentState.readingImages = false;
    int retArg;
    if (retArg = handle_args(argc, argv) != 0)
        return retArg;

    boost::asio::io_service::work work(ioService);
    processors = boost::thread::hardware_concurrency();

    while (!cin.eof()) handle_input();
    ioService.stop();
    threadpool.join_all();
    delete logReader;
    return 0;
}

void queue_work(std::function<void()> func) {
    ioService.post(boost::bind(func));
    while(threadpool.size() < processors)
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
}

/**
 * @class Command
 * @brief Describes a CLI Command that can be run in the program's repl
 */
class Command {
public:
    /**
     * @brief Constructor for a CLI command
     * @arg name Command name
     * @arg desc Command Description
     * @arg args List of argument names
     * @arg execute Function to execute when command is run, The vector of strings contains the arguments passed to the command at runtime
     */
    Command(string name, string desc, initializer_list<string> args, std::function<void(State&, vector<string>)> execute): name(name), desc(desc), args(args), execute(execute) { }
    string name, desc;
    vector<string> args;
    std::function<void(State&, vector<string>)> execute;
};

vector<Command> commands = {
    Command("help", "display this help message", {}, [=](State &newState, vector<string> args) {
        cout << "Commands:" << endl << endl;
        string space = string("");
        for (Command cmd : commands) {
            space.resize(20 - cmd.name.length() + 1 + boost::algorithm::join(cmd.args, " ").length(), ' ');
            cout << cmd.name << " " << boost::algorithm::join(cmd.args, " ") << space << " - " << cmd.desc << endl;
        }
    }),
    Command("log.trace", "sets log level to info", {}, [](State &newState, vector<string> args) {
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }),
    Command("log.info", "sets log level to info", {}, [](State &newState, vector<string> args) {
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    }),
    Command("log.debug", "sets log level to debug", {}, [](State &newState, vector<string> args) {
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
    }),
    Command("log.error", "sets log level to error", {}, [](State &newState, vector<string> args) {
        logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::error);
    }),
    Command("frames.start", "starts fetching frames", {}, [=](State &newState, vector<string> args) {
        if (!newState.readingImages) {
            newState.readingImages = true;
            importer.set_buffer_size(20);
        } else {
           BOOST_LOG_TRIVIAL(error) << "Frames are already being fetched";
        }
    }),
    Command("frames.stop", "starts fetching frames", {}, [=](State &newState, vector<string> args) {
        if (newState.readingImages) {
            newState.readingImages = false;
            importer.set_buffer_size(0);
        } else {
           BOOST_LOG_TRIVIAL(error) << "Frames are not being fetched";
        }
    }),
    Command("telemetry.file.add", "Adds file as new telemetry source", {"file"}, [=](State &newState, vector<string> args) {
        logReader->add_source(new MetadataReader(*logReader, args[0]));
        newState.hasMetadataSource = true;
    }),
    Command("telemetry.network.add", "Adds network address/port as new telemetry source", {"address", "port"}, [=](State &newState, vector<string> args) {
        logReader->add_source(new MetadataReader(*logReader, args[0], args[1]));
        newState.hasMetadataSource = true;
    }),
    Command("frames.source.pictures.add", "", {"path", "delay"}, [=](State &newState, vector<string> args) {
        if (logReader->num_sources() == 0) {
            BOOST_LOG_TRIVIAL(error) << "Cannot add image source until a metadata source has been specified";
        } else {
            importer.add_source(new PictureImport(args[0], logReader, goProFisheye), stol(args[1]));
            newState.hasImageSource = true;
        }
    }),
    Command("frames.source.videofile.add", "", {"path", "delay", "frameSkipMs"}, [=](State &newState, vector<string> args) {
        if (logReader->num_sources() == 0) {
            BOOST_LOG_TRIVIAL(error) << "Cannot add image source until a metadata source has been specified";
        } else {
            importer.add_source(new VideoImport(args[0], logReader, goProFisheye, stol(args[2])), stol(args[1]));
            newState.hasImageSource = true;
        }
    }),
#ifdef HAS_DECKLINK
    Command("frames.source.decklink.add", "", {"delay"}, [=](State &newState, vector<string> args) {
        if (logReader->num_sources() == 0) {
            BOOST_LOG_TRIVIAL(error) << "Cannot add image source until a metadata source has been specified";
        } else {
            importer.add_source(new DeckLinkImport(logReader, goProRect), stol(args[0]));
            newState.hasImageSource = true;
        }
    }),
#endif // HAS_DECKLINK
    Command("frames.source.remove", "Removes the source at the given index", {"index"}, [=](State &newState, vector<string> args) {
        if (importer.num_sources() <= stoi(args[0])) throw new std::runtime_error(string("Not a valid index: ") + args[0]);
        importer.remove_source(stoi(args[0]));
        newState.hasImageSource = importer.num_sources() > 0;
    }),
    Command("frames.source.list", "Lists frame sources", {}, [=](State &newState, vector<string> args) {
        cout << importer.source_descriptions() << endl;

    }),
    Command("frames.source.update_delay", "Updates the delay for the source at the given index", {"index", "delay"}, [=](State &newState, vector<string> args) {
        importer.update_delay(stoi(args[0]), stol(args[1]));
    })
};

void handle_state_change(State &newState) {
    if (!currentState.readingImages && newState.readingImages) {
        if (newState.hasImageSource && newState.hasMetadataSource) {
            currentState.readingImages = true;
            queue_work(assign_workers);
            queue_work(output);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Trying to read images without both image and metadata source is not supported";
        }
    }
    currentState.readingImages = newState.readingImages;
    currentState.hasMetadataSource = newState.hasMetadataSource;
    currentState.hasImageSource = newState.hasImageSource;
}

void handle_input() {
    cout << "wargcv$ ";
    string input;
    getline(cin, input);
    vector<string> args;
    boost::split(args, input, boost::is_any_of(" "));
    bool valid = false;

    for (Command cmd : commands) {
        if (args.size() > 0 && args[0].compare(cmd.name) == 0) {
            if (args.size() - 1 == cmd.args.size()) {
                BOOST_LOG_TRIVIAL(info) << "Executing command: " << cmd.name;
                State newState = currentState;
                try {
                    cmd.execute(newState, vector<string>(args.begin() + 1, args.end()));
                } catch (std::exception * e) {
                    BOOST_LOG_TRIVIAL(error) << e->what();
                    delete e;
                }
                handle_state_change(newState);
                valid = true;
                break;
            } else {
                cout << "Usage: " << endl;
                cout << cmd.name << " " << boost::algorithm::join(cmd.args, " ") << " - " << cmd.desc << endl;
            }
        }
    }
    if (!valid && !cin.eof()) {
        BOOST_LOG_TRIVIAL(info) << "Executing command: " << commands[0].name;
        commands[0].execute(currentState, vector<string>());
    }
}

int handle_args(int argc, char** argv) {
    try {
        po::options_description description("Usage: warg-cv [OPTION]");

        description.add_options()("help,h", "Display this help message")
            ("images,i", po::value<string>(), "Directory containing image files to be processed")
            ("video,v", po::value<int>(), "Video device to capture images from")
#ifdef HAS_DECKLINK
            ("decklink,d", "Use this option to capture video from a connected Decklink card")
#endif // HAS_DECKLINK
            ("telemetry,t", po::value<string>(), "Path of the telemetry log for the given image source")
            ("addr,a", po::value<string>(), "Address to connect to to recieve telemetry log")
            ("port,p", po::value<string>(), "Port to connect to to recieve telemetry log")
            ("output,o", po::value<string>(), "Directory to store output files; default is current directory")
            ("intermediate", "When this is enabled, program will output intermediary frames that contain objects of interest")
            ("videofile,f", po::value<string>(), "Path to video file to read frames from")
            ("videofeed,l", "If set, proggram will display results in real time");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        State newState = currentState;

        if (vm.count("help")) {
            cout << description << endl;
            return 1;
        }

        if (vm.count("telemetry")) {
            logReader->add_source(new MetadataReader(*logReader, vm["telemetry"].as<string>()));
            newState.hasMetadataSource = true;
            cout << "Adding Telemetry source..." << endl;
        }

        if (vm.count("addr") && vm.count("port")) {
            logReader->add_source(new MetadataReader(*logReader, vm["addr"].as<string>(), vm["port"].as<string>()));
            newState.hasMetadataSource = true;
            cout << "Adding Telemetry source..." << endl;
        }

#ifdef HAS_DECKLINK
        if (vm.count("decklink")) {
            importer.add_source(new DeckLinkImport(logReader, goProRect), 500);
            newState.hasImageSource = true;
            cout << "Adding video source..." << endl;
        }
#endif // HAS_DECKLINK

        if (vm.count("images")) {
            string path = vm["images"].as<string>();
            importer.add_source(new PictureImport(path, logReader, goProFisheye), 0);
            newState.hasImageSource = true;
            cout << "Adding picture source..." << endl;
        }

        if (vm.count("videofile")) {
            string path = vm["videofile"].as<string>();
            importer.add_source(new VideoImport(path, logReader, goProRect, 0), 100);
            newState.hasImageSource = true;
            cout << "Adding video source..." << endl;
        }

        if (vm.count("output")) {
            outputDir = vm["output"].as<string>();
        }

        if (vm.count("intermediate")) {
            intermediate = true;
        }

        if (vm.count("videofeed")) {
            videoFeed = true;
        }
        handle_state_change(newState);
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
