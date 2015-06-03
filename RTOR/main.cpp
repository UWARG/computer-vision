#include "RTOR.h"
#include <boost/lexical_cast.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;

frame * next_image();
void GetFilenames(vector<string> &file_names, string dir);
int handle_args(int argc, char** argv);
queue<frame *> in_buffer;
queue<frame *> display_buffer;
queue<string> out_buffer;

boost::asio::io_service ioService;
boost::thread_group threadpool;

VideoCapture cap;
vector<string> file_names;
int workers = 0;

string help_message = "Usage: rtor [OPTION]\n\
    -d=DIRECTORY    Parse images from DIRECTORY\n\
    -v=ID           Parse image from VideoCapture index ID\n";

void worker(frame *f)
{
    workers++;
	assert(!f->src.empty());
    f->run();
    display_buffer.push(f);
	workers--;
}

void read_images()
{
    frame *currentFrame;
    while(1)
    {
        if(workers < boost::thread::hardware_concurrency()){
			currentFrame = next_image();
			if(currentFrame != NULL)
			{
				string name;
				if(file_names.size() > 0)
				{
					name = file_names[0];
					log(1, "Reading Image " + name);
				}
				else
				{
					log(1, "Reading Image");
				}
				in_buffer.push(currentFrame);
			}
		}
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }
}

void assign_workers()
{
	frame * current;
    while(true)
    {
        if(in_buffer.size() > 0)
        {
            current = in_buffer.front();
            // spawn worker to process image;
            log(1, "Working...");
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


int main( int argc, char** argv )
{

    if(handle_args(argc, argv) == 1) return 0;
    init();

    int processors = boost::thread::hardware_concurrency();

    boost::thread read(read_images);
    boost::thread assn(assign_workers);
    boost::thread out(output);

	boost::asio::io_service::work work(ioService);
    for(int i = 0; i < processors; i++)
    {
        threadpool.create_thread(
            boost::bind(&boost::asio::io_service::run, &ioService)
        );
    }
	frame * current = NULL;
    while(true)
    {
		if(display_buffer.size() > 0)
        {
			if(current) {
				current->src.release();
				delete current;
			}
            current = display_buffer.front();
            if(&(current->src)== NULL || current->src.empty())
            {
                log(1, "Error: Image to display is empty. This should not happen");
                display_buffer.pop();
            }
            else
            {
                log(1, "Displaying image: " + current->ident);
                display(current);
                if(current->contours.size() > 0)
                {
                    stringstream out;
					for(int i = 0; i < current->contourData.size(); i++)
					{
						c_data * tmp = &(current->contourData[i]);
						out << tmp->centre.x << " " << tmp->centre.y
						<< " " << current->ident << endl;
					}
					out_buffer.push(out.str());
                    display_buffer.pop();
                }
                else
                {
                    display_buffer.pop();
                }

            }
        }
        waitKey(30);
    }

    // TODO: Output useful info about discovered targets in the frame
    assn.interrupt();
    read.interrupt();
    out.interrupt();
    return 0;
}

/**
 *  next_image() returns the next image to be processed
 */
frame * next_image()
{
    if(file_names.size() > 0)
    {
        Mat tmp_m = imread(file_names[0]);
        if(tmp_m.empty())
        {
            cout << "unable to read image: " << file_names[0] << endl;
            return NULL;
        }
        double ratio = ((double)tmp_m.cols)/tmp_m.rows;
        resize(tmp_m, tmp_m, Size((int)1080*ratio , 1080));
        //int video_timestamp = time(0);//cap.get( CV_CAP_PROP_POS_MSEC );
        int pos = file_names[0].find_last_of('/')+1;
        string name = file_names[0];
        if(pos != string::npos)
        {
            name = file_names[0].substr(pos);
        }
        frame *tmp = new frame(tmp_m, name);
        file_names.erase(file_names.begin());
        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        return tmp;
    } else
    {
        Mat tmp_m;
        cap >> tmp_m; // FIX: possibly reading the same frame over and over again
        if(tmp_m.empty()) return NULL;
        int video_timestamp = time(0);//cap.get( CV_CAP_PROP_POS_MSEC );
        frame *tmp = new frame(tmp_m, lexical_cast<string>(video_timestamp));
        return tmp;
    }

    return NULL;
}

struct recursive_directory_range
{
    typedef filesystem::directory_iterator iterator;
    recursive_directory_range(filesystem::path p) : p_(p) {}

    iterator begin() { return filesystem::directory_iterator(p_); }
    iterator end() { return filesystem::directory_iterator(); }

    filesystem::path p_;
};

void GetFilenames(vector<string> &file_names, string p)
{
	for (auto it : recursive_directory_range(p))
	{
		file_names.push_back(it.path().string());
		log(1, "Reading " + it.path().string());
    }
	log(1, "Sorting...");
    sort(file_names.begin(), file_names.end());
	file_names.erase(file_names.begin(),file_names.begin() + 2);


    /*DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        /*while ((ent = readdir (dir)) != NULL)
        {
            file_names.push_back(path + ent->d_name);
            log(1, "Reading " + lexical_cast<string>(ent->d_name));
        }
        file_names.erase(file_names.begin(),file_names.begin() + 2);
        sort(file_names.begin(), file_names.end());
        closedir (dir);
    }
    else
    {
        /* could not open directory */
    /*    perror (("Could not read directory: " + lexical_cast<string>(path)).c_str());
    }*/
}

int handle_args(int argc, char** argv)
{
    if(argc == 2)
    {
        if(argv[1][1] == 'd')
        {
            string filename;
            filename += argv[1];
            GetFilenames(file_names, filename.substr(3, string::npos));
        }
        else if (argv[1][1] == 'v')
        {
            if(true)//strlen(argv[1]) >= 3 && argv[1][3] >= '0' && argv[1][3] <= '9' )
            {
                cap.open(argv[1] + 2);
            }
            else
            {
                cout << "Invalid Identifier\n" << help_message;
                return 1;
            }
        }
        else
        {
            cout << "Illegal Argument\n" << help_message;
            return 1;
        }
    }
    else
    {
        cout << "WARG Real Time Object Recognition" << endl << help_message;
        return 1;
    }
    return 0;
}



