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
    vector<Point> * expected = new vector<Point>({Point(1445, 480), Point(1458, 405), Point(1535, 423), Point(1518, 496)});
    if (boost::unit_test::framework::master_test_suite().argc < 2) {
        BOOST_ERROR("Invalid number of arguments");
    }
    Mat input = imread(boost::unit_test::framework::master_test_suite().argv[1], cv::IMREAD_COLOR);
    Frame f(&input, "Test Image", Metadata());
    KMeansFilter filter;
    CannyContourCreator ccreator;
    Mat * filtered = filter.filter(f.get_img());
    vector<vector<Point> > * results = ccreator.get_contours(*filtered);
    BOOST_CHECK(true); // TODO: This test should be done by converting contours to binary mats and comparing overlap
    stringstream resultstr, expectedstr;
    resultstr << *results;
    expectedstr << *expected;
    BOOST_TEST_MESSAGE("RESULT: " << resultstr.str());
    BOOST_TEST_MESSAGE("EXPECTED: " << expectedstr.str());
}
