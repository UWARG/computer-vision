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
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <iostream>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"

using namespace cv;
using namespace std;
using namespace boost;

RNG rng(12345);
Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
Mat input, dst, filtered_dest, drawing, filtered;
int filter = 0, reductionFactor = 0, clusters = 9, attempts = 0, noiseReduction = 5;
vector<pair<Filter *, string>> filters;
CannyContourCreator ccreator;

void refresh_image() {
    filtered = *filters[filter].first->filter(input);
    vector<vector<Point> > * contours= ccreator.get_contours(filtered);

    /// Draw contours
    drawing = input.clone();
    for( int i = 0; i< contours->size(); i++ ) {
	drawContours( drawing, *contours, i, color, 2, 8);
    }
    Size size(900,600);
    resize(drawing, dst, size);
    resize(filtered, filtered_dest, size);

    imshow("Contours", dst);
    imshow("Filter", filtered_dest);
}

void on_filter_change( int, void* ){
    filters[filter].first->setParameter("reductionFactor",  reductionFactor + 1);
    filters[filter].first->setParameter("clusters", clusters + 1);
    filters[filter].first->setParameter("attempts",  attempts + 1);
    filters[filter].first->setParameter("noiseReduction",  noiseReduction);
    refresh_image();
}

int main(int argc, char ** argv) {
    if (argc < 2) {
	BOOST_LOG_TRIVIAL(error) << "Invalid number of arguments";
    }
    filters.push_back(pair<Filter *, string>(new KMeansFilter(), "KMeans1"));
    filters.push_back(pair<Filter *, string>(new KMeansFilter(), "KMeans2")); // must have at least two filters
    input = imread(argv[1], cv::IMREAD_COLOR);
    namedWindow("Contours", CV_WINDOW_AUTOSIZE );
    namedWindow("Filter", CV_WINDOW_AUTOSIZE );
    createTrackbar( "Filter", "Filter", &filter, filters.size() - 1, on_filter_change );
    createTrackbar( "Reduction Factor", "Filter", &reductionFactor, 10, on_filter_change );
    createTrackbar( "Cluster Count", "Filter", &clusters, 20, on_filter_change );
    createTrackbar( "Cluster Attempts", "Filter", &attempts, 9, on_filter_change );
    createTrackbar( "Noise Reduction", "Filter", &noiseReduction, 10, on_filter_change );

    while (true) {
	refresh_image();
	waitKey(0);
    }
}
