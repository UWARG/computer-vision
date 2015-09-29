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
	
#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

/**
  * @file target.h
  *
  * @brief Container class for storing information about
  *     identified targets in real-world measurements
  *     Adding PixelTargets consolidates their information
  *     into the Target
  *
  */

#include <opencv2/core/core.hpp>
#include <vector>

class PixelTarget;

class Target{
public:
    Target(std::string type);
   
    /**
     * @brief Getter for Target image
     *
     * @return The best image of the Target from one of the PixelTargets
     */
    cv::Mat get_image();

    /**
     * @brief Getter for type field
     *
     * @return Type std::string associated with the Target
     */
    std::string get_type();

    /**
     * @brief Getter for centroid
     *
     * @return GPS co-ordinates of the Target
     */
    cv::Point2f get_centroid();

    /**
     * @brief Getter for area
     *
     * @return Area of the Target in square metres
     */
    double get_area();

    /**
     * @brief Getter for perimeter
     *
     * @return Perimeter of the Target in metres
     */
    double get_perimeter();

    /**
     * @brief Getter for colour
     *
     * @return Average colour of the interiour of the Target
     */
    cv::Scalar get_colour();

    /**
     * @brief Getter for error
     *
     * @return 2D error magnitude of the Target's location in metres
     */
    cv::Point2f get_error();

    /**
     * @brief Getter for error angle
     *
     * @return Angle in radians between the direction of the error and North
     */
    double get_error_angle();

    /**
     * @brief Adds given PixelTarget to Target's storage
     *        and recalculate target information
     *
     * @param t PixelTarget to be added
     */
    void add_pixel_target(PixelTarget * t);

    /**
     * @brief Getter for pixel Targets
     *
     * @return Array containing all of the PixelTargets that were used to 
     *         create this instance of Target
     */
    PixelTarget * [] get_pixel_targets();
private:

    /**
     * @brief Cropped Image of the Target
     */
    cv::Mat image;

    /**
     * @brief current quality of the Image
     */
    double imageQuality;

    /**
     * @brief PixelTarget type description 
     */
    std::string type;

    /**
     * @brief GPS co-ordinates of the centre of the Target
     */
    cv::Point2f centroid;
    
    /**
     * @brief area of the target in square metres
     */
    double area;

    /**
     * @brief perimeter of the target in metres
     */
    double perimeter;
    
    /**
     * @brief Colour of the target in BGR (Blue, Green, Red) format
     */
    cv::Scalar colour;
    
    /**
     * @brief Calculated location error of the target as a 2D rectangle in metres
     */
    cv::Point2f error;
    
    /**
     * @brief Angle of the error as degrees clockwise from North
     */
    double error-angle;

    /**
     * @brief PixelTargets used to form this Target instance
     */
    std::vector<PixelTarget *> pixelTargets;
};


#endif // TARGET_H_INCLUDED
