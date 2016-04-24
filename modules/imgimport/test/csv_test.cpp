#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE imgimport
#include "frame.h"
#include "pictureimport.h"
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(csv_test){
    string telemetry_path="test_csv.csv";
    string filePath=boost::unit_test::framework::master_test_suite().argv[1];

    int count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir(filePath.c_str());
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) {
             count++;
        }
    }
    closedir(dirp);

    BOOST_REQUIRE(count > 0);

    ofstream fout(telemetry_path);
    fout<<"time,timeError,lat,lon,latError,lonError,pitch,roll,pitchRate,rollRate,yawRate,altitude,heading,altError,headingError,photonum"<<endl;
    for(int i=0;i<count;i++) {
        for(int j=0;j<15;j++){
            fout<<j<<",";
        }
        fout<<i<<endl;
    }
    PictureImport PI(telemetry_path,filePath);

    vector<Frame *> frames;
    while(1){
        Frame* show=PI.next_frame();
        if(show==NULL){
            break;
        }
        frames.push_back(show);
    }

    BOOST_REQUIRE(frames.size() == count);

    for (Frame * show : frames) {
        BOOST_CHECK(show->get_metadata()->time==0);
        BOOST_CHECK(show->get_metadata()->timeError==1);
        BOOST_CHECK(show->get_metadata()->lat==2);
        BOOST_CHECK(show->get_metadata()->lon==3);
        BOOST_CHECK(show->get_metadata()->latError==4);
        BOOST_CHECK(show->get_metadata()->lonError==5);
        BOOST_CHECK(show->get_metadata()->pitch==6);
        BOOST_CHECK(show->get_metadata()->roll==7);
        BOOST_CHECK(show->get_metadata()->pitchRate==8);
        BOOST_CHECK(show->get_metadata()->rollRate==9);
        BOOST_CHECK(show->get_metadata()->yawRate==10);
        BOOST_CHECK(show->get_metadata()->altitude==11);
        BOOST_CHECK(show->get_metadata()->heading==12);
        BOOST_CHECK(show->get_metadata()->altError==13);
        BOOST_CHECK(show->get_metadata()->headingError==14);
    }
}
