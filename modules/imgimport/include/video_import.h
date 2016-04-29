/**
 * @file video_import.h
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

#ifndef VIDEO_IMPORT_H_INCLUDED
#define VIDEO_IMPORT_H_INCLUDED

#include "imgimport.h"
#include "metadata_input.h"
#include "frame.h"
#include <opencv2/highgui/highgui.hpp>

/**
 * @class VideoImport
 *
 * Class for reading MP4 video files
 */
class VideoImport : public ImageImport {
public:
    /**
     *  @param videoFile video file to be read
     *  @param reader log reader containing telemetry data for the video
     */
    VideoImport(std::string videoFile, MetadataInput * reader);

    virtual Frame * next_frame();

private:
    cv::VideoCapture capture;
    double totalFrames;
    std::string fileName;
    double videoStartTime;
};

#endif // VIDEO_IMPORT_H_INCLUDED
