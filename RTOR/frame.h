#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "globals.h"

using namespace cv;

class frame{
public:
    Mat src;
    Mat diff;
    string ident;
    vector<vector<Point> > contours;
    vector<c_data> contourData;

    frame(Mat src, string ident);
    void run();
private:
    vector<Vec4i> hierarchy;

    void filter_image();
    void create_contours();
    void filter_contours();
};


#endif // FRAME_H_INCLUDED
