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

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "camera.h"

using namespace cv;

Camera::Camera(Size imageSize, Size sensorSize, Mat cameraMatrix, Mat distortionCoeffs)
        : cameraMatrix(cameraMatrix), distortionCoeffs(distortionCoeffs) {
    double fovx, fovy, focalLength, aspectRatio;
    Point2d principalPoint;
    calibrationMatrixValues(cameraMatrix, imageSize, sensorSize.width, sensorSize.height, fovx, fovy, focalLength, principalPoint, aspectRatio);
    fov = Point2d(fovx, fovy);
}

Mat * Camera::undistort(Mat & img) {
    Mat *tmp = new Mat();
    cv::undistort(*tmp, img, cameraMatrix, distortionCoeffs);
    return tmp;
}

Camera Camera::TestCamera() {
    double cameraMatrix[] = {
        2.4052826789763981e+03, 0, 2000,
        0, 2.4052826789763981e+03, 1500,
        0, 0, 1
    };

    double distMatrix[] = {
        6.0190515380007324e-02, -1.8618345553370965e+00, 0, 0,
        2.9590336363673964e+00
    };

    return Camera(
        Size(4000, 3000),
        Size(5.76, 4.29),
        Mat(
            Size(3, 3),
            CV_8UC1,
            cameraMatrix
        ),
        Mat(
            Size(5, 1),
            CV_8UC1,
            distMatrix
        )
    );
}

Point2d Camera::get_fov() {
    return fov;
}
