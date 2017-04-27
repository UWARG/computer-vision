/**
 * @file contour_filter.h
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

#ifndef CONTOUR_FILTER_H_INCLUDED
#define CONTOUR_FILTER_H_INCLUDED

#include <opencv2/core/core.hpp>

/**
 * @brief class for removing unwanted contours
 */
class ContourFilter {
public:
    /**
     * @brief Removes contours based on some implementation-specific metric
     */
    virtual void filter_contours(std::vector<std::vector<cv::Point> > &contours) = 0;
};

#endif // CONTOUR_FILTER_H_INCLUDED
