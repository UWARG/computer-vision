/**
 * @file pixel_object.h
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef PIXEL_OBJECT_H_INCLUDED
#define PIXEL_OBJECT_H_INCLUDED

/**
 * @class PixelObject
 *
 * @brief Container class for storing information about
 *        interesting objects in photos or video frames using pixels as measurement
 *
 */

#include <opencv2/core/core.hpp>
#include <vector>
#include "frame.h"

class PixelObject{
public:
    PixelObject(cv::Mat & crop, std::vector<cv::Point> & contour, cv::Point2d centroid, double area, double perimeter, cv::Scalar colour, cv::Point2d error, double errorAngle);

    /**
     * @brief Getter for contour
     * @return locations of the vertices of the object
     */
    std::vector<cv::Point> & get_contour();

    /**
     * @brief Getter for centroid
     *
     * @return Pixel location of the Object in its frame
     */
    cv::Point2d get_centroid();

    /**
     * @brief Getter for area
     *
     * @return Area of the Object in pixels as it appears in its frame
     */
    double get_area();

    /**
     * @brief Getter for perimeter
     *
     * @return Perimeter of the Object in pixels as it appears in its frame
     */
    double get_perimeter();

    /**
     * @brief Getter for colour
     *
     * @return Average colour of the interiour of the Object's contour
     */
    cv::Scalar get_colour();
    
    /**
     * @brief Getter for GPS centroid
     *
     * @return GPS location of the Object in its frame
     */
    cv::Point2d get_gps_centroid();

    /**
     * @brief Setter for GPS centroid
     *
     * @param gps The GPS centroid of the Object in its frame
     */
    void set_gps_centroid(cv::Point2d gps);

    /**
     * @brief Getter for area
     *
     * @return Area of the Object in meters
     */
    double get_gps_area();


    /**
     * @brief Getter for error
     *
     * @return 2D error magnitude of the Object's location
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
     * @return Frame associated with this Object
     */
    Frame * get_image();

    /**
     * @brief Getter for cropped image
     *
     * @return Cropped image of the object associated with this Object
     */
    cv::Mat & get_cropped_image();

    /// add_object sets the PixelObject's frame pointer
    friend void Frame::add_object(PixelObject * o);
private:
    /**
     * @brief Pixel locations of the vertices of the Object
     */
    std::vector<cv::Point> contour;

    /**
     * @brief Pixel location of the centre of the PixelObject
     */
    cv::Point2d centroid;
    
    /**
     * @brief GPS location of the centre of the PixelObject. This is calculated
     * from PixelObject::centroid after targetanalysis.
     */
    cv::Point2d gps_centroid;


    /**
     * @brief area of the target in pixels
     */
    double area;

    /**
     * @brief area of the target in meters. This is computed after
     * targetanalysis has been completed.
     */
    double gps_area;


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
     * @brief Frame described by this Object
     */
    Frame * image;

    /*
     * @brief Cropped picture of object
     */
    cv::Mat crop;
    
};


#endif // PIXEL_OBJECT_H_INCLUDED
