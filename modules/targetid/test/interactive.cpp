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
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"
#include "histogram.h"

using namespace cv;
using namespace std;
using namespace boost;
using namespace filesystem;

RNG rng(129384);
Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
Mat input, dst, filtered_dest, drawing, filtered;
int filter = 0, reductionFactor = 3, clusters = 9, attempts = 0, noiseReduction = 5;
vector<pair<Filter *, string>> filters;
CannyContourCreator ccreator;
vector<string> fileNames;
int fileIndex = 0;
double frameTime;

void refresh_image() {
    timer t;
    filtered = *filters[filter].first->filter(input);
    vector<vector<Point> > * contours= ccreator.get_contours(filtered);
    frameTime = t.elapsed();

    /// Draw contours
    drawing = input.clone();
    for( int i = 0; i< contours->size(); i++ ) {
        drawContours( drawing, *contours, i, color, 2, 8);
    }
    Size size(960,640);
    resize(drawing, dst, size);
    resize(filtered, filtered_dest, size);

    imshow("Contours", dst);
    imshow("Filter", filtered_dest);
    try {
        displayOverlay("Contours", "Render Time: " + lexical_cast<string>((int)(frameTime * 1000)) + " ms", 0);
    } catch (...) { } // Catch exception thrown if OpenCV is not built with QT
}

void on_filter_change( int, void* ){
    filters[filter].first->setParameter("reductionFactor",  reductionFactor + 1);
    filters[filter].first->setParameter("clusters", clusters + 1);
    filters[filter].first->setParameter("attempts",  attempts + 1);
    filters[filter].first->setParameter("noiseReduction",  noiseReduction);
    refresh_image();
}

void on_file_change(int, void*) {
    input = imread(fileNames[fileIndex], cv::IMREAD_COLOR);
    refresh_image();
}

struct recursive_directory_range
{
    typedef filesystem::directory_iterator iterator;
    recursive_directory_range(filesystem::path p) : p_(p) {}

    iterator begin() { return filesystem::directory_iterator(p_); }
    iterator end() { return filesystem::directory_iterator(); }

    filesystem::path p_;
};


int main(int argc, char ** argv) {
    if (argc < 2) {
        BOOST_LOG_TRIVIAL(error) << "Invalid number of arguments";
    }
    filters.push_back(pair<Filter *, string>(new KMeansFilter(), "KMeans"));
    filters.push_back(pair<Filter *, string>(new HistFilter(), "HistFilter")); // must have at least two filters

    path p(argv[1]);
    BOOST_LOG_TRIVIAL(info) << "Opening Directory " << p.string();
    if (!is_directory(p)) {
        exit(1);
    }

	for (auto it : recursive_directory_range(p)) {
        if(is_regular_file(it.path())) fileNames.push_back(it.path().string());
        BOOST_LOG_TRIVIAL(info) << "Reading Image " << it.path().string();
        input = imread(it.path().string(), cv::IMREAD_COLOR);
        filters[1].first->filter(input); // HistFilter relies on accumulative data
    }
    sort(fileNames.begin(), fileNames.end());
    on_file_change(0, 0);

    namedWindow("Contours", CV_WINDOW_NORMAL );
    namedWindow("Filter", CV_WINDOW_NORMAL );
    createTrackbar( "Filter", "Filter", &filter, filters.size() - 1, on_filter_change );
    createTrackbar( "Reduction Factor", "Filter", &reductionFactor, 10, on_filter_change );
    createTrackbar( "Cluster Count", "Filter", &clusters, 20, on_filter_change );
    createTrackbar( "Cluster Attempts", "Filter", &attempts, 9, on_filter_change );
    createTrackbar( "Noise Reduction", "Filter", &noiseReduction, 10, on_filter_change );
    createTrackbar( "Image", "Contours", &fileIndex, fileNames.size(), on_file_change );

    while (true) {
        refresh_image();
        waitKey(0);
    }
}
