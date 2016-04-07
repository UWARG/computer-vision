/**
 * @file contour_comparison.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#include <opencv2/imgproc.hpp>
#include <boost/log/trivial.hpp>
#include <boost/test/unit_test.hpp>
#include <vector>
#include "contour_comparison.h"

using namespace cv;
using namespace std;

double compare_contours(vector<vector<Point> > & contourA, vector<vector<Point> > & contourB) {
    BOOST_LOG_TRIVIAL(trace) << "Comparing contours";
    if (contourA.size() == 0 || contourB.size() == 0) return 0;
    Rect boundsA;
    Rect boundsB;
    for(int i = 0; i < contourA.size(); i++) boundsA |= boundingRect(contourA[i]);
    for(int i = 0; i < contourB.size(); i++) boundsB |= boundingRect(contourB[i]);
    Size largest = Size(boundsA.x + boundsA.width > boundsB.x + boundsB.width
                        ? boundsA.x + boundsA.width : boundsB.x + boundsB.width,
                        boundsA.y + boundsA.height > boundsB.y + boundsB.height
                        ? boundsA.y + boundsA.height : boundsB.y + boundsB.height);
    Mat imgA = Mat::zeros(largest, CV_8U);
    Mat imgB = Mat::zeros(largest, CV_8U);
    BOOST_LOG_TRIVIAL(trace) << "Drawing contours";
    vector<vector<Point> >hullA(contourA.size()), hullB(contourB.size());

    for(int i = 0; i < hullA.size(); i++) {
        convexHull( Mat(contourA[i]), hullA[i], false );
        drawContours(imgA, hullA, i, Scalar(255,255,255), FILLED);
    }
    for(int i = 0; i < hullB.size(); i++) {
        convexHull( Mat(contourB[i]), hullB[i], false );
        drawContours(imgB, hullB, i, Scalar(255,255,255), FILLED);
    }
    BOOST_LOG_TRIVIAL(trace) << "Comparing images";
    Mat diff = imgA & imgB;
    int countA = countNonZero(imgA);
    int countB = countNonZero(imgB);
    double countDiff = countNonZero(diff);
    double base = countA > countB ? countA : countB;
    BOOST_LOG_TRIVIAL(info) << "Size(contourA) = " << countA << ", Size(contourB) = " << countB << " Size(difference) = " << countDiff;
    BOOST_LOG_TRIVIAL(info) << "Result: " << (countDiff/base);
    return countDiff/base;
}
