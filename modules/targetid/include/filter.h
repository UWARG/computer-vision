/**
 * @file filter.h
 * @author WARG
 *
 * @section LICENSE
 *
 * Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 * All rights reserved.
 *
 * This software is licensed under a modified version of the BSD 3 clause license
 * that should have been included with this software in a file called COPYING.txt
 * Otherwise it is available at:
 * https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <map>
#include <string>


/** @class Filter
 *
 *  @brief Abstract class providing an interface to modify images before edge detection is applied.
 *
 *  Any particular Filter will be run only once on each image, allowing incremental data to be gathered
 *  by a Filter.
 */
class Filter {
public:
	Filter();
	virtual ~Filter();

    /**
     *  @brief run the filter on a given image
     *
     *  Filters should be implemented to enhance potential targets in the image so that
     *  they can be more easily picked up by an edge detector and supress other features of
     *  the image
     *  The filter should not make decisions on what constitutes a target.
     *  Any unusual object in the frame should be considered a target for the
     *  purposes of the filter
     *
     *  @return A pointer to a new filtered image
     */
    virtual cv::Mat * filter(const cv::Mat & src) = 0;

    /**
     *  @brief Sets parameter in internal map
     *
     *  These parameters are used for doing on the fly adjustments
     *  to a filter during testing. There is no guarantee that a
     *  given filter will use any given parameter.
     */
	void setParameter(std::string param, int value);

protected:
	std::map<std::string, int> * parameters;
};

#endif // FILTER_H_INCLUDED
