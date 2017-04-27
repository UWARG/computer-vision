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
#include <cstdlib>
#include <boost/date_time.hpp>

using namespace cv;
using namespace std;
using namespace boost;
using namespace boost::filesystem;
namespace bt = boost::posix_time;

static const int HAS_FFPROBE = std::system("which ffprobe > /dev/null") == 0;

static const std::locale dateFormat = std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%dT%H:%M:%sZ"));

std::time_t pt_to_time_t(const bt::ptime& pt) {
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;
}

VideoImport::VideoImport(string videoFile, MetadataInput * reader, Camera &camera, long frameSkipMs) : ImageImport(reader, camera), frameSkipMs(frameSkipMs) {
    capture.open(videoFile);
    if (!capture.isOpened()) {
        BOOST_LOG_TRIVIAL(error) << "Cannot open video file " << videoFile;
        throw std::exception();
    }
    capture.set(CAP_PROP_CONVERT_RGB, true);
    totalFrames = capture.get(CAP_PROP_FRAME_COUNT);
    if (HAS_FFPROBE) {
        string cmd = "ffprobe -v error -show_entries stream_tags=creation_time:format_tags=creation_time -of default=noprint_wrappers=1:nokey=1 -i " + videoFile + "| head -n 1";

        FILE*           fp;
        const int       SIZEBUF = 1234;
        char            buf [SIZEBUF];
        if ((fp = popen(cmd.c_str (), "r")) != NULL) {
            std::string  cur_string = "";
            while (fgets(buf, sizeof (buf), fp)) {
                cur_string += buf;
            }
            string result = cur_string.substr (0, cur_string.size () - 1);
            BOOST_LOG_TRIVIAL(trace) << "Video start date: " << result;
            pclose(fp);

            bt::ptime pt;
            std::istringstream is(result);
            is.imbue(dateFormat);
            is >> pt;
            videoStartTime = pt_to_time_t(pt);
            BOOST_LOG_TRIVIAL(trace) << "Video start MS from ffprobe creation date: " << videoStartTime;
        }
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Reading video but ffprobe is not available\nvideo frame times will be based on the last write time of the file and may be inaccurate";
        // Last write time should roughly equal the end of the video, so work backwards using video length to get the start time
        videoStartTime = last_write_time(videoFile) - totalFrames/capture.get(CAP_PROP_FPS);
        BOOST_LOG_TRIVIAL(trace) << "Video start MS from last_write_time: " << videoStartTime;
    }
}

Frame * VideoImport::next_frame() {
    Mat * frame = new Mat();
    double prevPos = capture.get(CAP_PROP_POS_MSEC);
    bool success = capture.read(*frame);

    if (!success) {
        BOOST_LOG_TRIVIAL(info) << "No more video frames, stopping.";
        return NULL;
    }

    double pos = capture.get(CAP_PROP_POS_MSEC);
    time_t frameTime = videoStartTime + pos / 1000;
    tm * utcTime = gmtime(&frameTime);

    double time = utcTime->tm_hour * 10000 + utcTime->tm_min * 100 + utcTime->tm_sec + ((int)floor(pos) % 1000)/1000;

    capture.set(CAP_PROP_POS_MSEC, prevPos + frameSkipMs);
    return new Frame(frame, fileName + boost::lexical_cast<string>(time) + ".jpg", reader != NULL ? reader->get_metadata(time) : Metadata(), camera);
}

std::string VideoImport::to_string() {
    return "VideoImport";
}
