/**
 * @file goose_filte.rcpp
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
#include <math.h>
#include "goose_filter.h"

#define PI 3.1415926

using namespace std;
using namespace cv;

void GooseFilter::filter_contours(vector<vector<Point> > &contours) {
    int removed = 0;
    for (int i = 0; i < contours.size(); i++) {
        vector<Point> &contour = contours[i];
        bool keep = false;
        RotatedRect bounds = minAreaRect(contour);
        if (bounds.size.area() > 0) {
            double aspectRatio = bounds.size.width / bounds.size.height;
            double area = contourArea(contour);
            double extent = area / bounds.size.area();
            vector<Point> hull;
            convexHull(contour, hull);
            double hullArea = contourArea(hull);
            double solidity = area/hullArea;
            double equiDiameter = sqrt(4*area/PI);
            RotatedRect ellipse = contour.size() > 4 ? fitEllipse(contour) : bounds;

            keep = aspectRatio > 0.2 && aspectRatio < 5 &&
                extent > 0.6 &&
                solidity > 0.9 &&
                ellipse.size.area() / bounds.size.area() < 1.2;

        } else {
            keep = false;
        }
        if (!keep) {
            contours.erase(contours.begin() + i);
            i--;
            removed++;
        }
    }
    BOOST_LOG_TRIVIAL(debug) << "Removed " << removed << " contours";
}
