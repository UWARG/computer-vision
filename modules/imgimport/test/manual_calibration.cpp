/*
 *   This file is part of WARG's computer-vision
 *
 *  Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <vector>
#include <iostream>
#include "frame.h"
#include "pictureimport.h"

using namespace cv;
using namespace std;
using namespace boost;
using namespace filesystem;
namespace logging = boost::log;

double matrix[] = {
    2.4052826789763981e+03, 0, 2000,
    0, 2.4052826789763981e+03, 1500,
    0, 0, 1
};

double newmatrix[] = {
    2000, 0, 2000,
    0, 2000, 1500,
    0, 0, 1
};

double distortion[] = {
    -0.392, 0.146, 0, 0, -0.023
};

int scale;


int intDistortion[5];

Camera camera(
    Size(4000, 3000),
    Size2d(120, 90),
    Mat(
        Size(3, 3),
        CV_8UC1,
        matrix
    ),
    Mat(
        Size(5, 1),
        CV_8UC1,
        distortion
    ),
    Mat(
        Size(3, 3),
        CV_64F,
        newmatrix
    )

);

vector<Frame*> frames;
int fileIndex = 0;

void on_file_change(int, void*) {
    distortion[0] = (intDistortion[0] - 500)/1000.0;
    distortion[1] = (intDistortion[1] - 500)/1000.0;
    distortion[2] = (intDistortion[2] - 500)/1000.0;
    distortion[3] = (intDistortion[3] - 500)/1000.0;
    distortion[4] = (intDistortion[4] - 500)/1000.0;
    newmatrix[0] = (scale - 500)*10.0;
    newmatrix[4] = (scale - 500)*10.0;
    Camera camera(
        Size(4000, 3000),
        Size2d(120, 90),
        Mat(
            Size(3, 3),
            CV_64F,
            matrix
        ),
        Mat(
            Size(5, 1),
            CV_64F,
            distortion
        ),
        Mat(
            Size(3, 3),
            CV_64F,
            newmatrix
        )
    );
    frames[fileIndex]->undistort(camera);

    Mat dst;
    Mat drawing = frames[fileIndex]->get_img().clone();
    Size size(960,640);
    resize(drawing, dst, size);

    imshow("image", dst);
}

int main(int argc, char ** argv) {

    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);
    if (argc < 2) {
        BOOST_LOG_TRIVIAL(error) << "Invalid number of arguments";
    }

    PictureImport importer(argv[1], NULL, camera);
    Frame *current = NULL;
    while ((current = importer.next_frame()) != NULL) {
        frames.push_back(current);
    }

    on_file_change(0, 0);

    namedWindow("image", CV_WINDOW_NORMAL);
    createTrackbar("Image", "image", &fileIndex, frames.size() - 1, on_file_change);
    createTrackbar("k0", "image", &intDistortion[0], 1000, on_file_change);
    createTrackbar("k1", "image", &intDistortion[1], 1000, on_file_change);
    createTrackbar("k2", "image", &intDistortion[2], 1000, on_file_change);
    createTrackbar("k3", "image", &intDistortion[3], 1000, on_file_change);
    createTrackbar("k4", "image", &intDistortion[4], 1000, on_file_change);
    createTrackbar("scale", "image", &scale, 1000, on_file_change);

    while (true) {
        waitKey(0);
    }
}
