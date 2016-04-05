/**
 * @file contour_comparison.h
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

#ifndef CONTOUR_COMPARISON_H_INCLUDED
#define CONTOUR_COMPARISION_H_INCLUDED

#include <opencv2/imgproc.hpp>
#include <vector>

/**
 *  @brief Compares two contours and returns their overlap
 *
 *  The return value uses the larger of the two contour sets as the base
 *  This means that, for example, if the first set of contours encloses the second but is twice
 *  as large, the resulting value will be 0.5, which is the same value as if the first was only
 *  half the size as the second
 *
 *  @param contourA first contour to compare
 *  @param contourB second contour to compare
 *  @return Overlap between the two Contours, 1 being a complete match, 0 being a complete mismatch
 */
double compare_contours(std::vector<std::vector<cv::Point> > & contourA, std::vector<std::vector<cv::Point> > & contourB);

#endif // CONTOUR_COMPARISON_H_INCLUDED
