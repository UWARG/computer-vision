/**
 * @file camera.h
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


#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

/**
 * @class Camera
 *
 * @brief Container class for storing information about
 *     the camera used to capture an image *
 */


class Camera {
public:
    /**
     * @brief creates a Camera representing the specs for a given lens and sensor
     *
     * @param imageSize Size of the camera image
     * @param fov The field of view of the camera
     * @param cameraMatrix OpenCV camera calibration matrix giving focal length and principal point
     * @param distortionCoeffs OpenCV distortion coefficients used for undistorting images
     * #param undistort Whether or not to apply undistortion to images. If false the camera acts as a placeholder that can be used for testing
     */
    Camera(cv::Size imageSize, cv::Size2d fov, cv::Mat cameraMatrix, cv::Mat distortionCoeffs, cv::Mat newCameraMatrix, bool applyUndistort = true);

    /**
     * @brief Returns the field of view of the camera in degrees for both horizontal and vertical dimensions
     */
    cv::Size2d get_fov();

    /**
     * @brief Undistorts image
     * @param img mat to be undistorted
     * @return An orthorectified version of the given mat
     */
    cv::Mat * undistort(cv::Mat &img);

    /**
     * @brief Example Camera for testing purposes
     */
    static Camera TestCamera();

private:
    bool applyUndistort;
    cv::Size2d fov;
    cv::Mat cameraMatrix;
    cv::Mat distortionCoeffs;
    cv::Mat newCameraMatrix;
};

#endif //CAMERA_H_INCLUDED
