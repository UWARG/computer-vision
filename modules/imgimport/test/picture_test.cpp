#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE imgimport
#include "frame.h"
#include "pictureimport.h"
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <fstream>
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
    ofstream fout(telemetry_path);
    fout<<"time,timeError,lat,lon,latError,lonError,pitch,roll,pitchRate,rollRate,yawRate,altitude,heading,altError,headingError,photonum"<<endl;
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<15;j++)
            fout<<j<<",";
        fout<<i<<endl;
    }
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

