/*
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
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
#include <boost/program_options.hpp>
#include <queue>
#include <iostream>
#include <fstream>
#include "frame.h"
#include "target_identifier.h"
#include "imgimport.h"
#include "vidimport.h"
#include "pictureimport.h"
#include "metadata_input.h"
#include "target.h"

using namespace std;
using namespace boost;
using namespace cv;
namespace logging = boost::log;
namespace po = boost::program_options;

const int BUFFER_SIZE = 20;

Frame* next_image();
int handle_args(int argc, char** argv);
queue<Frame*> in_buffer;
queue<Target*> out_buffer;
queue<Frame*> intermediate_buffer;

boost::asio::io_service ioService;
boost::thread_group threadpool;

vector<string> file_names;
int workers = 0;
bool hasMoreFrames = true;
string outputDir = "./";
bool intermediate = false;

// Processing module classes
ImageImport * importer = NULL;
TargetIdentifier identifier;
MetadataInput * logReader = NULL;

double aveFrameTime = 1000;
int frameCount = 0;

void worker(Frame* f) {
    workers++;
    assert(!f->get_img().empty());
    identifier.process_frame(f);
    if (intermediate && f->get_objects().size() > 0) {
        intermediate_buffer.push(f);
    }

    workers--;
}

void read_images() {
    Frame* currentFrame;
    while (hasMoreFrames) {
        if (in_buffer.size() < BUFFER_SIZE) {
            Frame* f = importer->next_frame();
            if (f) {
                in_buffer.push(f);
            }
            else {
                hasMoreFrames = false;
            }
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void assign_workers() {
    Frame* current;
    while (hasMoreFrames || in_buffer.size() > 0) {
        if (in_buffer.size() > 0) {
            current = in_buffer.front();
            // spawn worker to process image;
            BOOST_LOG_TRIVIAL(info) << "Working...";
            ioService.post(boost::bind(worker, current));
            in_buffer.pop();
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void output() {
    filebuf fb;
    while (hasMoreFrames || out_buffer.size() > 0 || intermediate_buffer.size() > 0 || workers > 0) {
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
            f->save(outputDir);
            intermediate_buffer.pop();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
    ioService.stop();
}

void init() {
    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
}

int main(int argc, char** argv) {
    init();
    int retArg;
    if (retArg = handle_args(argc, argv) != 0)
        return retArg;

    int processors = boost::thread::hardware_concurrency();

    ioService.post(boost::bind(read_images));
    ioService.post(boost::bind(assign_workers));
    ioService.post(boost::bind(output));

    boost::asio::io_service::work work(ioService);
    for (int i = 0; i < processors; i++) {
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
    }
    threadpool.join_all();
    delete logReader;
    delete importer;
    return 0;
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
            ("output,o", po::value<string>(), "Directory to store output files; default is current directory")
            ("intermediate", "When this is enabled, program will output intermediary frames that contain objects of interest");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, description), vm);
        po::notify(vm);

        if (vm.count("help") || vm.size() == 0) {
            cout << description << endl;
            return 1;
        }

        int devices = vm.count("video") + vm.count("decklink") + vm.count("images");
        if (devices > 1) {
            cout << "Invalid options: You can only specify one image source at a time" << endl;
            return 1;
        } else if(devices == 0) {
            cout << "Error: You must specify an image source!" << endl;
            return 1;
        }
        if (!vm.count("telemetry")) {
            cout << "Invalid options; You must specify a telemetry file" << endl;
            return 1;
        }
        string telemetry = vm["telemetry"].as<string>();

#ifdef HAS_DECKLINK
        if (vm.count("decklink")) {
            importer = new VideoImport();
        }
#endif // HAS_DECKLINK

        if (vm.count("images")) {
            string path = vm["images"].as<string>();
            BOOST_LOG_TRIVIAL(debug) << "Reading Telemetry Log file at path " << path;
            logReader = new MetadataInput(telemetry);
            importer = new PictureImport(path, logReader);
        }

        if (vm.count("output")) {
            outputDir = vm["output"].as<string>();
        }

        if (vm.count("intermediate")) {
            intermediate = true;
        }
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
