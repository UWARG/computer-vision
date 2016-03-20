#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TargetIdentification
#include "qr_identifier.h"
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(qr_test){
    string root_path = boost::unit_test::framework::master_test_suite().argv[1];
    unique_ptr<string> result;
    BOOST_LOG_TRIVIAL(info) << "Reading files in dir: " << root_path;

    /// Negative Test
    Mat img = imread(root_path + "/IMG_1644.JPG",CV_LOAD_IMAGE_COLOR);
    BOOST_REQUIRE(!img.empty());
    result = qr_identifier(img);
    BOOST_CHECK(result == nullptr);

    /// Positive Test
    Mat img2 = imread(root_path + "/qr_code_testdata/test2.jpg",CV_LOAD_IMAGE_COLOR);
    BOOST_REQUIRE(!img2.empty());
    result = qr_identifier(img2);
    BOOST_REQUIRE(result != nullptr);
    BOOST_CHECK(result->compare("http://www.qrstuff.com/")==0);
}
