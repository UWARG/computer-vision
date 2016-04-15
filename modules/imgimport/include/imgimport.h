/**
 * @file imgimport.h
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

#ifndef IMAGE_IMPORT_H_INCLUDED
#define IMAGE_IMPORT_H_INCLUDED


#include "frame.h"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

/**
 * @class ImageImport
 *
 * @brief Abstract class for importing frames and corresponding metadata
 *
 */

class ImageImport {
    public:
        /**
         * @brief Creates a ImageImport for the telemetry file at the given path
         *
         * @param path Path of the telemetry file
         * @param photoPath directory containing image and video files to import is required but does not need to contain photos
         * @param videoDeviceNums indices of the video devices from which to read video frames
         */
        ImageImport(); 
        virtual ~ImageImport();

        /**
         * @brief Retrieves the next frame to be analyzed
         *
         * @return Frame to be analyzed
         */
        virtual Frame * next_frame() = 0;
};

#endif // IMAGE_IMPORT_H_INCLUDED
