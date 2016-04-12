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
#include <dirent.h>
using namespace std;
using namespace cv;
using namespace boost;

BOOST_AUTO_TEST_CASE(hist_test){
    string filePath=boost::unit_test::framework::master_test_suite().argv[1];
    HistFilter test_filter;
    DIR* dr;
    dr=opendir(filePath.c_str());
    struct dirent* drnt;
    for(drnt=readdir(dr);drnt!=NULL;drnt=readdir(dr)){
        Mat src;
        string true_path=filePath+'/'+drnt->d_name;
        src=imread(true_path,CV_LOAD_IMAGE_COLOR);
        while(src.empty()){
    	    drnt=readdir(dr);
            if(drnt==NULL){
                break;
            }
            if(strcmp(drnt->d_name,"..")==0||strcmp(drnt->d_name,".")==0){
                continue;
            }
            string true_path=filePath+'/'+drnt->d_name;
            src=imread(true_path,CV_LOAD_IMAGE_COLOR);
        }
        if(drnt==NULL){
            break;
        }
        const Mat buffer=src;
        test_filter.filter(buffer);
    }
    Mat* show;
    DIR* dir;
    dir=opendir(filePath.c_str());
    struct img_vertices{
        vector<vector<Point>> vertices;
        string name;
    };
    img_vertices expected_contours[24];
    expected_contours[0].vertices.push_back({vector<Point>({Point(44,242), Point(59,250), Point(50,265), Point(37,256)})});
    expected_contours[0].name="IMG_1547.jpg";
    expected_contours[1].name="IMG_1552.jpg";
    expected_contours[2].name="IMG_1557.jpg";
    expected_contours[3].name="IMG_1559.jpg";
    expected_contours[4].name="IMG_1577.jpg";
    expected_contours[5].vertices.push_back({vector<Point>({Point(44,242), Point(59,250), Point(50,265), Point(37,256)})});
    expected_contours[5].name="IMG_1579.jpg";
    expected_contours[6].name="IMG_1581.jpg";
    expected_contours[7].name="IMG_1583.jpg";
    expected_contours[8].name="IMG_1595.jpg";
    expected_contours[9].vertices.push_back({vector<Point>({Point(634,1316), Point(653,1350), Point(581,1388), Point(566,1347)})});
    expected_contours[9].vertices.push_back({vector<Point>({Point(629,1334), Point(636,1346), Point(591,1369), Point(582,1355)})});
    expected_contours[9].vertices.push_back({vector<Point>({Point(1274,957), Point(1283,965), Point(1266,980), Point(1257,970)})});
    expected_contours[9].name="IMG_1598.jpg";
    expected_contours[10].vertices.push_back({vector<Point>({Point(635,481), Point(686,572), Point(586,626), Point(536,533)})});
    expected_contours[10].vertices.push_back({vector<Point>({Point(628,499), Point(670,568), Point(589,612), Point(552,538)})});
    expected_contours[10].vertices.push_back({vector<Point>({Point(616,535), Point(623,540), Point(619,550), Point(631,558), Point(625,567),Point(614,562), Point(603,576), Point(595,570), Point(600,557), Point(591,550),Point(598,540), Point(607,543)})});
    expected_contours[10].name="IMG_1616.jpg";
    expected_contours[11].name="IMG_1616(1).jpg";
    expected_contours[12].name="IMG_1631.jpg";
    expected_contours[13].vertices.push_back({vector<Point>({Point(1781,1246), Point(1824,1294), Point(1780,1338), Point(1736,1288)})});
    expected_contours[13].vertices.push_back({vector<Point>({Point(1781,1263), Point(1807,1294), Point(1782,1318), Point(1755,1288)})});
    expected_contours[13].name="IMG_1632.jpg";
    expected_contours[14].vertices.push_back({vector<Point>({Point(197,303), Point(246,337), Point(205,381), Point(156,343)})});
    expected_contours[14].vertices.push_back({vector<Point>({Point(200,319), Point(226,340), Point(201,364), Point(175,343)})});
    expected_contours[14].name="IMG_1635.jpg";
    expected_contours[15].vertices.push_back({vector<Point>({Point(612,657), Point(637,659), Point(636,681), Point(610,677)})});
    expected_contours[15].name="IMG_1642.jpg";
    expected_contours[16].name="IMG_1644.jpg";
    expected_contours[17].vertices.push_back({vector<Point>({Point(972,50), Point(1009,85), Point(934,143), Point(895,110)})});
    expected_contours[17].vertices.push_back({vector<Point>({Point(969,67), Point(988,84), Point(935,126), Point(917,109)})});
    expected_contours[17].vertices.push_back({vector<Point>({Point(1936,138), Point(1956,158), Point(1912,189), Point(1888,171)})});
    expected_contours[17].vertices.push_back({vector<Point>({Point(1927,155), Point(1932,160), Point(1916,173), Point(1910,168)})});
    expected_contours[17].vertices.push_back({vector<Point>({Point(1548,508), Point(1550,515), Point(1538,524), Point(1528,526),Point(1528,516),Point(1535,510)})});
    expected_contours[17].name="IMG_1670.jpg";
    expected_contours[18].name="IMG_1673.jpg";
    expected_contours[19].name="IMG_1888.jpg";
    expected_contours[20].vertices.push_back({vector<Point>({Point(1634,1112), Point(1655,1147), Point(1590,1197), Point(1565,1158)})});
    expected_contours[20].vertices.push_back({vector<Point>({Point(1628,1130), Point(1634,1140), Point(1592,1174), Point(1584,1163)})});
    expected_contours[20].name="IMG_1894.jpg";
    expected_contours[21].vertices.push_back({vector<Point>({Point(1202,182), Point(1294,184), Point(1302,269), Point(1209,265)})});
    expected_contours[21].vertices.push_back({vector<Point>({Point(1214,192), Point(1287,195), Point(1292,258), Point(1221,255)})});
    expected_contours[21].vertices.push_back({vector<Point>({Point(1244,209), Point(1254,217), Point(1263,212), Point(1271,218), Point(1262,228),Point(1268,234), Point(1262,239), Point(1254,233), Point(1246,238), Point(1239,231),Point(1244,223), Point(1239,216)})});
    expected_contours[21].name="IMG_1899.jpg";
    expected_contours[22].vertices.push_back({vector<Point>({Point(1261,970), Point(1302,1025), Point(1251,1059), Point(1216,1006)})});
    expected_contours[22].vertices.push_back({vector<Point>({Point(820,1084), Point(943,1418), Point(932,1428), Point(813,1191)})});
    expected_contours[22].vertices.push_back({vector<Point>({Point(596,1329), Point(716,1499), Point(695,1499), Point(587,1337)})});
    expected_contours[22].vertices.push_back({vector<Point>({Point(776,1333), Point(785,1343), Point(781,1353), Point(797,1361), Point(788,1374),Point(780,1367), Point(768,1383), Point(760,1376), Point(764,1362), Point(756,1354),Point(760,1348), Point(768,1351)})});
    expected_contours[22].name="IMG_1908.jpg";
    expected_contours[23].name="IMG_1913.jpg";
    for(drnt=readdir(dir);drnt!=NULL;drnt=readdir(dir)){
        Mat src;
        string true_path=filePath+'/'+drnt->d_name;
        src=imread(true_path,CV_LOAD_IMAGE_COLOR);
        while(src.empty()){
    	    drnt=readdir(dir);
            if(drnt==NULL){
                break;
            }
            if(strcmp(drnt->d_name,"..")==0||strcmp(drnt->d_name,".")==0){
                continue;
            }
            string true_path=filePath+'/'+drnt->d_name;
            src=imread(true_path,CV_LOAD_IMAGE_COLOR);
        }
        if(drnt==NULL){
            break;
        }
        vector<vector<Point>> expected;
        for(int i=0;i<24;i++){
            if(expected_contours[i].name.compare(drnt->d_name)==0)
                expected=expected_contours[i].vertices;
        }
        const Mat buffer=src;
        show=test_filter.filter(buffer);
        CannyContourCreator ccreator;
        vector<vector<Point> > * results = ccreator.get_contours(*show);
        double diff = compare_contours(*results, expected);
        BOOST_CHECK(diff > 0.01);
        BOOST_TEST_MESSAGE("RESULT: " << diff);
    }
}
