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

using namespace cv;
using namespace std;
using namespace boost;

VideoImport::VideoImport(string videoFile, MetadataInput * reader) : ImageImport(reader) {
    capture.open(videoFile);
    if (!capture.isOpened()) {
        BOOST_LOG_TRIVIAL(error) << "Cannot open video file " << videoFile;
        throw std::exception();
    }
    capture.set(CAP_PROP_CONVERT_RGB, true);
    totalFrames = capture.get(CAP_PROP_FRAME_COUNT);
    fileName = videoFile.find_last_of('/') == string::npos ? videoFile : videoFile.substr(videoFile.find_last_of('/'));
    // TODO: Read video modified time from filesystem
}

Frame * VideoImport::next_frame() {
    Mat * frame = new Mat();
    bool success = capture.read(*frame);

    if (!success) {
        BOOST_LOG_TRIVIAL(info) << "No more video frames, stopping.";
        return NULL;
    }

    double pos = capture.get(CAP_PROP_POS_MSEC);
    double frameTime = videoStartTime + pos;

    // TODO: Use video modified time and video position to find frame time

    const posix_time::ptime now = posix_time::microsec_clock::local_time();

    const posix_time::time_duration td = now.time_of_day();

    const long hours        = td.hours();
    const long minutes      = td.minutes();
    const long seconds      = td.seconds();
    const long milliseconds = td.total_milliseconds() -
                              ((hours * 3600 + minutes * 60 + seconds) * 1000);
    double time = hours * 10000 + minutes * 100 + seconds + ((double)milliseconds)/1000;

    return new Frame(frame, fileName + boost::lexical_cast<string>(time) + ".jpg", reader->get_metadata(time));
}
