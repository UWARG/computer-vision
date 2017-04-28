/**
 * @file frame.h
 * @author WARG
 *
 * @brief Container class for storing photos or frames of video to be
 *        processed and analyzed
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at: https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include <opencv2/core/core.hpp>
#include "metadata.h"
#include <vector>
#include "camera.h"

class PixelObject;

class Frame{
public:
    Frame(cv::Mat * img, std::string id, Metadata m, Camera &camera);

    ~Frame();
    /**
     * @brief 
     *
     * @return timestamp identifier for the frame
     */
    std::string get_id();

    /**
     * @brief 
     *
     * @return image associated with the frame
     */
    cv::Mat & get_img();

    /**
     * @brief Original Image
     * @return original image before any calibration
     */
    cv::Mat & orig_img();

    /**
     * @brief Adds given object to the list of objects in the frame
     *
     * @param o object identified as being in the frame
     */
    void add_object(PixelObject * o);

    /**
     * @brief getter for targets
     *
     * @return iterator pointing at the first target in the target list
     */
    std::vector<PixelObject *> & get_objects();

    /** 
     * @brief getter for metadata
     *
     * @return const pointer to internal metadata structure
     */
    const Metadata * get_metadata();

    /**
     * @brief Stores the frame as an image file
     * @param dir Directory to store the file
     */
    void save(std::string dir);

    /**
     * @brief Getter for the pixel distance
     *
     * @return The distance covered by each pixel of the image in the X and Y
     * directions.
     */
    cv::Point2d get_pixel_distance(); 

    /**
     * @brief Setter for the pixel distance
     *
     * @param The distance covered by each pixel of the image in the X and Y
     * directions.
     */
    void set_pixel_distance(cv::Point2d); 

    /**
     * @brief Setter for the pixel distance
     *
     * @param The distance covered by each pixel of the image in the X direction.
     * @param The distance covered by each pixel of the image in the Y
     * direction.
     */
    void set_pixel_distance(double x, double y); 

    /*
     * @brief returns a new undistorted image using the given camera
     * Intended for testing purposes, Does not modify the Frame.
     * @param camera the camera to use for undistortion
     * @return A new image that is the original image contained in this frame undistorted with the camera
     */
    cv::Mat* undistort(Camera &camera);

private:
    
    /**
     * @brief image associated with the Frame 
     */
    cv::Mat * img;

    /**
     * @brief original image without undistortion
     */
    cv::Mat * origImg;

    /**
     * @brief Timestamp for the frame
     */
    std::string id;

    /**
     * @brief Data collected from telemetry about the plane when the image was captured
     */
    Metadata data;

    /**
     * @brief Targets that appear in the frame
     */
    std::vector<PixelObject *> objects;

    /**
     * @brief Distance of each pixel covers in the x and y direction with
     * componesation for altitude, camera lens, etc.
     */
    cv::Point2d pixel_distance; 

    /*
     * @brief Camera used to capture the frame
     */
    Camera &camera;
};


#endif // FRAME_H_INCLUDED
