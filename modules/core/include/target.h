/**
 * @file target.h
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

/**
 * @class Target
 *
 * @brief Container class for storing information about
 *     identified targets in real-world measurements
 *     Adding PixelTargets consolidates their information
 *     into the Target
 *
 */

#include <opencv2/core/core.hpp>
#include <vector>

class Object;

class Target{
public:
    Target();

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
    cv::Point2d get_centroid();

    /**
     * @brief Getter for the pixel distance
     *
     * @return The distance covered by each pixel of the image in the X and Y
     * directions.
     */
    cv::Point2d get_pixel_distance();


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
    void add_object(Object * t);

    /**
     * @brief Getter for pixel Targets
     *
     * @return Array containing all of the PixelTargets that were used to
     *         create this instance of Target
     */
    const std::vector<Object *> & get_objects();

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
    cv::Point2d centroid;
    
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
    double errorAngle;

    /**
     * @brief Distinct Objects used to form this Target instance
     */
    std::vector<Object *> objects;
};


#endif // TARGET_H_INCLUDED
