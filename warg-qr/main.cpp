#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <zbar.h>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
using namespace zbar;
using namespace boost;

vector<string> file_paths;
vector<string> file_names;
bool d = false;
string help_message = "Usage: warg-qr [OPTION]\n\
    -d=DIRECTORY    Parse images from DIRECTORY\n\
    -v=ID           Parse image from VideoCapture index ID\n";


VideoCapture cap;

struct frame
{
    Mat image;
    string filename;
    bool empty_image;
};

frame empty_frame = {Mat(), "", true};

/**
*  next_image() returns the next image to be processed
*/
frame next_image()
{
    if(d)
    {
        if(file_paths.size() == 0)return empty_frame;
        Mat tmp_m = imread(file_paths[0]);
        if(tmp_m.empty())
        {
            cout << "unable to read image: " << file_paths[0] << endl;
            return empty_frame;
        }
        frame tmp = {tmp_m , file_paths[0], false};
        file_paths.erase(file_paths.begin());
        return tmp;
    }
    else
    {
        Mat tmp_m;
        cap >> tmp_m;
        if(tmp_m.empty()) return empty_frame;
        int video_timestamp = time(0);//cap.get( CV_CAP_PROP_POS_MSEC );
        frame tmp = {tmp_m, lexical_cast<string>(video_timestamp), false};
        return tmp;
    }

    return empty_frame;
}

struct recursive_directory_range
{
    typedef filesystem::directory_iterator iterator;
    recursive_directory_range(filesystem::path p) : p_(p) {}

    iterator begin()
    {
        return filesystem::directory_iterator(p_);
    }
    iterator end()
    {
        return filesystem::directory_iterator();
    }

    filesystem::path p_;
};

void GetFilenames(vector<string> &file_paths, string p)
{
    for (auto it : recursive_directory_range(p))
    {
        file_paths.push_back(it.path().string());
        cout << "Reading " << it.path().string() <<endl;
    }
    sort(file_paths.begin(), file_paths.end());
    //file_paths.erase(file_paths.begin(),file_paths.begin() + 2);
}

int handle_args(int argc, char** argv)
{
    file_paths = vector<string>();
    if(argc == 2)
    {
        if(argv[1][1] == 'd')
        {
            string filename;
            filename += argv[1];
            GetFilenames(file_paths, filename.substr(3, string::npos));
            d = true;
        }
        else if (argv[1][1] == 'v')
        {
            if(strlen(argv[1]) >= 3 && argv[1][3] >= '0' && argv[1][3] <= '9' )
            {
                cap.open(argv[1][3] - 48);
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
        cout << "WARG QR Detector" << endl << help_message;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    handle_args(argc, argv);
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    namedWindow("Image",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    frame f = next_image();
    while (!f.empty_image)
    {
        Mat grey;
        cvtColor(f.image,grey,CV_BGR2GRAY);
        int width = f.image.cols;
        int height = f.image.rows;
        uchar *raw = (uchar *)grey.data;
        // wrap image data
        Image image(width, height, "Y800", raw, width * height);
        // scan the image for barcodes
        int n = scanner.scan(image);
        // extract results
        for(Image::SymbolIterator symbol = image.symbol_begin();
                symbol != image.symbol_end();
                ++symbol)
        {
            vector<Point> vp;
            // do something useful with results
            cout << "decoded " << symbol->get_type_name() << " symbol \"" << symbol->get_data() << '"' <<" in " << f.filename << endl;
            frame * current;
            filebuf fb;
            fb.open("out.txt", ios::app);
            ostream out(&fb);
            out << symbol->get_type_name() << " " << symbol->get_data() << " " << f.filename.substr(f.filename.find_last_of('/') + 1) <<endl;
            fb.close();



            int n = symbol->get_location_size();
            for(int i=0; i<n; i++)
            {
                vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
            }
            RotatedRect r = minAreaRect(vp);
            Point2f pts[4];
            r.points(pts);
            for(int i=0; i<4; i++)
            {
                line(f.image,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
            }
            //cout<<"Angle: "<<r.angle<<endl;
        }
        double ratio = ((double)f.image.cols)/f.image.rows;
        resize(f.image, f.image, Size((int)640*ratio , 640));
        imshow("Image", f.image); //show the frame in "MyVideo" window
        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
        f = next_image();
    }
    return 0;
}



