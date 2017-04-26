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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TargetIdentification
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"
#include "benchmark.h"
#include "contour_comparison.h"
#include "object_detector.h"
#include "pixel_object.h"

using namespace cv;
using namespace std;
using namespace boost;

ostream & operator<<(ostream & out, vector<Point_<int> > & contour) {
    for(int i = 0; i < contour.size(); i++) {
        out << contour.at(i);
    }
    return out;
}

ostream & operator<<(ostream & out, vector<vector<Point_<int> > > & contour) {
    for(int i = 0; i < contour.size(); i++) {
        out << "{" << endl;
        out << "    " << contour.at(i) << endl;
        out << "}" << endl;
    }
    return out;
}

BOOST_AUTO_TEST_CASE(KMeansAndCanny) {
    vector<vector<Point> > * expected = new vector<vector<Point> >({vector<Point>({Point(1445, 480), Point(1458, 405), Point(1535, 423), Point(1518, 496), Point(1445, 480)})});
    if (boost::unit_test::framework::master_test_suite().argc < 2) {
        BOOST_ERROR("Invalid number of arguments");
    }
    Mat input = imread(boost::unit_test::framework::master_test_suite().argv[1], cv::IMREAD_COLOR);
    Camera camera = Camera::TestCamera();
    Frame f(&input, "Test Image", Metadata(), camera);
    KMeansFilter * filter = new KMeansFilter();
    CannyContourCreator * ccreator = new CannyContourCreator();
    ObjectDetector detector(filter, ccreator);

    detector.process_frame(&f);
    vector<vector<Point> > results;
    for (PixelObject * o : f.get_objects()) {
        results.push_back(o->get_contour());
        BOOST_TEST_MESSAGE("Found object of size " << o->get_area());
        BOOST_TEST_MESSAGE("Perimeter: " << o->get_perimeter());
        BOOST_TEST_MESSAGE("Colour: " << o->get_colour());
    }

    double diff = compare_contours(results, *expected);
    BOOST_CHECK(diff > 0.01); // ensures that at least something is detected
    BOOST_TEST_MESSAGE("RESULT: " << diff);

    benchmark_function("k_means", [&]() { filter->filter(f.get_img()); }, 10);
    Mat * filtered = filter->filter(f.get_img());
    benchmark_function("Canny", [&]() { ccreator->get_contours(*filtered); }, 10);
    delete filtered;
    delete filter;
    delete ccreator;
    delete expected;
}
