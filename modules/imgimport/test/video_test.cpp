/**
 * @file video_test.cpp
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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ImageImport

#include <boost/test/unit_test.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include "video_import.h"

using namespace std;
using namespace boost;

namespace logging = boost::log;

BOOST_AUTO_TEST_CASE(VideoFileTest){
    if(boost::unit_test::framework::master_test_suite().argc <= 1) {
        BOOST_ERROR("Invalid number of arguments");
    }

    Camera camera = Camera::TestCamera();
    VideoImport * v = new VideoImport(boost::unit_test::framework::master_test_suite().argv[1], NULL, camera, 1000);
    Frame *f = v->next_frame();
    const Metadata *m = f->get_metadata();
    BOOST_CHECK(f->get_img().rows > 0);
    delete v;
    delete f;
}
