#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE imgimport
#include "frame.h"
#include "pictureimport.h"
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(picture_test){
    vector<int> n;
    n.push_back(3);
    string telemetry_path=boost::unit_test::framework::master_test_suite().argv[2];
    string filePath=boost::unit_test::framework::master_test_suite().argv[1];
    PictureImport PI(telemetry_path,filePath,n);
    DIR* dr=opendir(filePath.c_str());
    struct dirent* drnt;
    while(1){
        Frame* show=PI.next_frame();
        if(show==NULL){
            break;
        }
        Mat picture_stored=show->get_img();
        drnt=readdir(dr);
        Mat original_picture;
        while(original_picture.empty())
            original_picture=imread(drnt->d_name,CV_LOAD_IMAGE_COLOR);
        Mat diff;
        compare(picture_stored,original_picture,diff,cv::CMP_NE);
        int nz=countNonZero(diff);
        BOOST_CHECK(nz==0);
    }
}

BOOST_AUTO_TEST_CASE(csv_test){
    vector<int> n;
    n.push_back(3);
    string telemetry_path=boost::unit_test::framework::master_test_suite().argv[2];
    string filePath=boost::unit_test::framework::master_test_suite().argv[1];
    PictureImport PI(telemetry_path,filePath,n);
    for(int i=0;1;i++){
        Frame* show=PI.next_frame();
        if(show==NULL){
            break;
        }
        BOOST_CHECK(show->get_metadata()->time==readcsv(telemetry_path.c_str()).at(i).time);
        BOOST_CHECK(show->get_metadata()->timeError==readcsv(telemetry_path.c_str()).at(i).timeError);
        BOOST_CHECK(show->get_metadata()->lat==readcsv(telemetry_path.c_str()).at(i).lat);
        BOOST_CHECK(show->get_metadata()->lon==readcsv(telemetry_path.c_str()).at(i).lon);
        BOOST_CHECK(show->get_metadata()->latError==readcsv(telemetry_path.c_str()).at(i).latError);
        BOOST_CHECK(show->get_metadata()->lonError==readcsv(telemetry_path.c_str()).at(i).lonError);
        BOOST_CHECK(show->get_metadata()->pitch==readcsv(telemetry_path.c_str()).at(i).pitch);
        BOOST_CHECK(show->get_metadata()->roll==readcsv(telemetry_path.c_str()).at(i).roll);
        BOOST_CHECK(show->get_metadata()->pitchRate==readcsv(telemetry_path.c_str()).at(i).pitchRate);
        BOOST_CHECK(show->get_metadata()->rollRate==readcsv(telemetry_path.c_str()).at(i).rollRate);
        BOOST_CHECK(show->get_metadata()->yawRate==readcsv(telemetry_path.c_str()).at(i).yawRate);
        BOOST_CHECK(show->get_metadata()->altitude==readcsv(telemetry_path.c_str()).at(i).altitude);
        BOOST_CHECK(show->get_metadata()->heading==readcsv(telemetry_path.c_str()).at(i).heading);
        BOOST_CHECK(show->get_metadata()->altError==readcsv(telemetry_path.c_str()).at(i).altError);
        BOOST_CHECK(show->get_metadata()->headingError==readcsv(telemetry_path.c_str()).at(i).headingError);
    }
}
