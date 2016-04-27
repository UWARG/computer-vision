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
#include <opencv2/imgproc/imgproc.hpp>
#include "benchmark.h"

using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(picture_test){
    vector<int> n;
    n.push_back(3);
    string telemetry_path=boost::unit_test::framework::master_test_suite().argv[2];
    telemetry_path+="/test_csv.csv";
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

    MetadataInput* mdin=new MetadataInput(telemetry_path);
    PictureImport PI(filePath,n,mdin);
    DIR* dr=opendir(filePath.c_str());
    struct dirent* drnt;
    vector<Frame *> frames;
    while(1){
        Frame* show=PI.next_frame();
        if(show==NULL){
            break;
        }
        frames.push_back(show);
    }

    Metadata m = mdin->get_metadata(221000.5);
    BOOST_CHECK(abs(m.time - 221000.59375) < 0.00001);
    BOOST_CHECK(abs(m.lat - 49.907920000110124) < 0.00001);
    BOOST_CHECK(abs(m.lon - -98.2732866668043) < 0.00001);
    BOOST_CHECK(abs(m.heading - 318) < 0.00001);
    BOOST_CHECK(abs(m.altitude - 103.313) < 0.01);
    BOOST_TEST_MESSAGE("Altitude " << m.altitude);

    benchmark_function("Metadata Lookup", [&]() { m = mdin->get_metadata((rand() % 6514) + 215410); }, 1000);

    BOOST_REQUIRE(frames.size() == count);

    for (Frame * show : frames) {
        Mat picture_stored=show->get_img();
        Mat original_picture;
        while(original_picture.empty()){
            drnt=readdir(dr);
            string true_path=filePath+'/'+drnt->d_name;
            original_picture=imread(true_path,CV_LOAD_IMAGE_COLOR);
        }
        Mat diff;
        compare(picture_stored,original_picture,diff,cv::CMP_NE);
        Mat grey;
        cvtColor(diff,grey,CV_BGR2GRAY);
        int nz=countNonZero(grey);
        BOOST_CHECK(nz==0);
    }
    delete mdin;
}

