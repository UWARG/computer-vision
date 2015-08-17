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
#include "target-identification/target_identifier.h"
#include "image-metadata-matcher/metadata_matcher.h"

using namespace std;
using namespace boost;
using namespace cv;
namespace logging = boost::log;
namespace po = boost::program_options;

Frame * next_image();
int handle_args(int argc, char** argv);
queue<Frame *> in_buffer;
queue<Target *> out_buffer;


boost::asio::io_service ioService;
boost::thread_group threadpool;

vector<string> file_names;
int workers = 0;

string help_message = "Usage: warg-cv [OPTION]";

// Processing module classes
MetadataMatcher matcher("telemetry.csv");
TargetIdentifier identifier;

void worker(Frame *f)
{
    workers++;
	assert(!f->get_img().empty());
    identifier.process_frame(f);

	workers--;
}

void read_images()
{
    Frame *currentFrame;
    while(1)
    {
		in_buffer.push(matcher.next_frame());
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void assign_workers()
{
	Frame * current;
    while(true)
    {
        if(in_buffer.size() > 0)
        {
            current = in_buffer.front();
            // spawn worker to process image;
            BOOST_LOG_TRIVIAL(info) << "Working...";
            ioService.post(boost::bind(worker, current));
            in_buffer.pop();
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void output()
{
    filebuf fb;
    while(true)
    {
        if(out_buffer.size() > 0)
        {
            fb.open("out.txt", ios::app);
            ostream out(&fb);

            out << out_buffer.front();
            // Output
            out_buffer.pop();
            fb.close();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void init()
{
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}


int main( int argc, char** argv )
{
    init();
    if(handle_args(argc, argv) == 1) return 0;

    int processors = boost::thread::hardware_concurrency();

    ioService.post(boost::bind(read_images));
    ioService.post(boost::bind(assign_workers));
    ioService.post(boost::bind(output));

	boost::asio::io_service::work work(ioService);
    for(int i = 0; i < processors; i++)
    {
        threadpool.create_thread(
            boost::bind(&boost::asio::io_service::run, &ioService)
        );
    }
    threadpool.join_all();
    return 0;
}

int handle_args(int argc, char ** argv){
    return 0; 
}
