/*This file is part of WARG's computer-vision.

Copyright (c) 2015, WARG
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

4. Any code used MUST be referenced to WARG (Waterloo Aerial Robotics Group). Additionally, this code cannot be used in any competition against WARG.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	*/
	
#ifndef PIXEL_TARGET_H_INCLUDED
#define PIXEL_TARGET_H_INCLUDED

/**
 * @file pixel_target.h
 *
 * @brief Container class for storing information about
 *        targets located in photos or video frames using pixels as measurement
 *
 */

#include <opencv2/core/core.hpp>
#include <vector>

class Frame;

class PixelTarget{
public:
    PixelTarget(std::string type, cv::Point2d centroid, double area, double perimeter, cv::Scalar colour, cv::Point2d error, double errorAngle);
    
    /**
     * @brief Getter for type field
     *
     * @return Type std::string associated with the PixelTarget
     */
    std::string get_type();

    /**
     * @brief Getter for centroid
     *
     * @return Pixel location of the PixelTarget in its frame
     */
    cv::Point2d get_centroid();

    /**
     * @brief Getter for area
     *
     * @return Area of the PixelTarget in pixels as it appears in its frame
     */
    double get_area();

    /**
     * @brief Getter for perimeter
     *
     * @return Perimeter of the PixelTarget in pixels as it appears in its frame
     */
    double get_perimeter();

    /**
     * @brief Getter for colour
     *
     * @return Average colour of the interiour of the PixelTarget's contour
     */
    cv::Scalar get_colour();

    /**
     * @brief Getter for error
     *
     * @return 2D error magnitude of the PixelTarget's location
     */
    cv::Point2d get_error();

    /**
     * @brief Getter for error angle
     *
     * @return Angle in radians between the direction of the error and the vertical
     */
    double get_error_angle();

    /**
     * @brief Getter for image
     *
     * @return Frame associated with this PixelTarget
     */
    Frame * get_image();
private:
    /**
     * @brief PixelTarget type description 
     */
    std::string type;

    /**
     * @brief Pixel locations of the vertices of the PixelTarget
     */
    std::vector<cv::Point> contour;
    
    /**
     * @brief Pixel location of the centre of the PixelTarget
     */
    cv::Point2d centroid;
    
    /**
     * @brief area of the target in pixels
     */
    double area;

    /**
     * @brief perimeter of the target in pixels 
     */
    double perimeter;
    
    /**
     * @brief Colour of the target in BGR (Blue, Green, Red) format
     */
    cv::Scalar colour;
    
    /**
     * @brief Calculated location error of the target as a 2D rectangle in pixels
     */
    cv::Point2d error;
    
    /**
     * @brief Angle of the error as radians clockwise from vertical
     */
    double errorAngle;

    /**
     * @brief Frame described by this PixelTarget
     */
    Frame * image;
};


#endif // PIXEL_TARGET_H_INCLUDED
