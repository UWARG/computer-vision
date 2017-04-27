/**
 * @file goose_filter.h
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

#ifndef GOOSE_FILTER_H_INCLUDED
#define GOOSE_FILTER_H_INCLUDED

#include <opencv2/core/core.hpp>
#include "contour_filter.h"

/**
 * @brief class for removing contours that do not resemble geese
 */
class GooseFilter : public ContourFilter {
public:
    /*
     * @brief removes contours that are not shaped like geese
     */
    virtual void filter_contours(std::vector<std::vector<cv::Point> > &contours);
};

#endif // CONTOUR_FILTER_H_INCLUDED
