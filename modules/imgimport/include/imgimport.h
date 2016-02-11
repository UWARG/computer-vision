/* 
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code 
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used 
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
	
#ifndef IMAGE_IMPORT_H_INCLUDED
#define IMAGE_IMPORT_H_INCLUDED


#include "frame.h"
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include "csvreading.h"

using namespace std::vector;

/** 
 * @class ImageImport
 *
 * @brief Module for matching frames with telemetry information that 
 *        corresponds to them
 *
 * Reads Photographs and Video frames and creates a corresponding Frame object, adding 
 *  metadata information from a telemetry file
 * Handles image and video files as well as live video devices
 *
 * @section Notes   
 *
 * Ideally this should use both the timestamp in the image's
 * exif metadata and the time that the plane records sending
 * the signal to take a picture plus whatever delay that
 * exists between when the signal is sent to the camera and
 * when the camera takes the photograph.
 *
 */

class ImageImport {
    public:
        /**
         * @brief Creates a ImageImport for the telemetry file at the given path
         *

         *
         * @param path Path of the telemetry file
         * @param photoPath directory containing image and video files to import is required but does not need to contain photos
         * @param videoDeviceNums indices of the video devices from which to read video frames
         */
        ImageImport(std::string telemetry_path, std::string filePath, std::vector<int> videoDeviceNums);

        ~ImageImport();

        /**
         * @brief Retrieves the next frame to be analyzed
         *
         * @return Frame to be analyzed
         */
        Frame * next_frame();
    private:
	DIR* dr;
	Metadatalinkedlist* mdls;
	struct dirent* drnt;
	vector<int> videoDeviceNums;
	Frame* frame_buffer;
};

#endif // IMAGE_IMPORT_H_INCLUDED
