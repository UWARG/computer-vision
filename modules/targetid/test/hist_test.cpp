#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TargetIdentification
#include "histogram.h"
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"
#include "benchmark.h"
#include "contour_comparison.h"
using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(hist_test){
    vector<int> n;
    n.push_back(3);
    string telemetry_path=boost::unit_test::framework::master_test_suite().argv[2];
    string filePath=boost::unit_test::framework::master_test_suite().argv[1];
    ofstream fout(telemetry_path);
    fout<<"time,timeError,lat,lon,latError,lonError,pitch,roll,pitchRate,rollRate,yawRate,altitude,heading,altError,headingError,photonum"<<endl;
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<15;j++){
            fout<<j<<",";
        }
        fout<<i<<endl;
    }
    hist_filter test_filter;
    PictureImport* input=new PictureImport(telemetry_path,filePath,n);
    Frame* src;
    for(src=input->next_frame();src!=NULL;src=input->next_frame()){
        const Mat buffer=src->get_img();
        test_filter.filter(buffer);
    }
    delete input;
    PictureImport* in=new PictureImport(telemetry_path,filePath,n);
    Mat* show;
    for(src=in->next_frame();src!=NULL;src=in->next_frame()){
        const Mat buffer=src->get_img();
        show=test_filter.filter(buffer);
        KMeansFilter filter;
        CannyContourCreator ccreator;
        Mat * filtered = filter.filter(buffer);
        vector<vector<Point> > * results = ccreator.get_contours(*filtered);
        vector<vector<Point> > * expected = ccreator.get_contours(*show);
        double diff = compare_contours(*results, *expected);
        BOOST_CHECK(diff > 0.01);
        BOOST_TEST_MESSAGE("RESULT: " << diff);
/*        namedWindow("original",WINDOW_NORMAL);
        resizeWindow("original", 900, 900);
        imshow("original",buffer);
        namedWindow("display",WINDOW_NORMAL);
        resizeWindow("display", 900, 900);
        imshow("display",*show);
        waitKey(0);*/
    }
}
