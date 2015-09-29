/*This file is part of WARG's computer-vision.

    computer-vision is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. You cannot however, claim it as your own. 
	Any code used MUST be referenced to WARG (Waterloo Aerial Robotics Group). 
	Additionally, this code cannot be used in any competition against WARG.

    computer-vision is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with computer-vision.  If not, see <http://www.gnu.org/licenses/>.
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
