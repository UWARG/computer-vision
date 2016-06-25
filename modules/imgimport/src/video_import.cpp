/**
 * @file video_import.cpp
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

#include "video_import.h"
#include <boost/log/trivial.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <ctime>

using namespace cv;
using namespace std;
using namespace boost;
using namespace boost::filesystem;

VideoImport::VideoImport(string videoFile, MetadataInput * reader, Camera &camera) : ImageImport(reader, camera) {
    capture.open(videoFile);
    if (!capture.isOpened()) {
        BOOST_LOG_TRIVIAL(error) << "Cannot open video file " << videoFile;
        throw std::exception();
    }
    capture.set(CAP_PROP_CONVERT_RGB, true);
    totalFrames = capture.get(CAP_PROP_FRAME_COUNT);
    fileName = videoFile.find_last_of('/') == string::npos ? videoFile : videoFile.substr(videoFile.find_last_of('/'));
    videoStartTime = last_write_time(fileName);
}

Frame * VideoImport::next_frame() {
    Mat * frame = new Mat();
    bool success = capture.read(*frame);

    if (!success) {
        BOOST_LOG_TRIVIAL(info) << "No more video frames, stopping.";
        return NULL;
    }

    double pos = capture.get(CAP_PROP_POS_MSEC);
    time_t frameTime = videoStartTime + pos / 1000;
    tm * utcTime = gmtime(&frameTime);

    double time = utcTime->tm_hour * 10000 + utcTime->tm_min * 100 + utcTime->tm_sec + ((int)floor(pos) % 1000)/1000;

    return new Frame(frame, fileName + boost::lexical_cast<string>(time) + ".jpg", reader->get_metadata(time), camera);
}
