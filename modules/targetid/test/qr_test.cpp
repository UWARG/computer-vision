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
    Mat img=imread(boost::unit_test::framework::master_test_suite().argv[1],CV_LOAD_IMAGE_COLOR);
    string result=qr_identifier(img);
    BOOST_CHECK(result.compare("http://www.qrstuff.com/")==0);
}
