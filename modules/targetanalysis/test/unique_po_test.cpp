/**
 * @file unique_po_test.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */
            
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TargetAnalysis

#include <boost/test/unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/property_tree/ptree.hpp>

#include "target_analyzer.h"
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define PIXEL_OBJECTS_PER_IMAGE 7

using namespace std;
using namespace boost;
using namespace cv;

namespace logging = boost::log;
using namespace boost::filesystem;

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


BOOST_AUTO_TEST_CASE(UniquePOTest){
    if(boost::unit_test::framework::master_test_suite().argc <= 1) {
        BOOST_ERROR("Invalid number of arguments");
    }

    string root_path = boost::unit_test::framework::master_test_suite().argv[1];
    string description = boost::unit_test::framework::master_test_suite().argv[2];    
    
    //Read directory
    BOOST_REQUIRE( filesystem::exists( root_path ) );
    directory_iterator end_itr; // default construction yields past-the-end iterator
    int fileCount = std::count_if(directory_iterator(root_path),directory_iterator(),static_cast<bool(*)(const
                path&)>(is_regular_file));
   
   
    int numImage = 0;
    int numPixelObjects = 0;
    PixelObject* pointerList[fileCount * PIXEL_OBJECTS_PER_IMAGE];
    KMeansFilter * filter = new KMeansFilter();
    CannyContourCreator * ccreator = new CannyContourCreator();

    for ( directory_iterator itr( root_path ); itr != end_itr; ++itr ){
        if ( is_regular_file(itr->path()) ){
            string current_file = itr->path().string();
            if (has_suffix(current_file, ".jpg") || has_suffix(current_file,
            ".jpeg")){
                
                //Manually generate a PixelObject
                cv::Mat* img = new cv::Mat(imread(current_file,CV_LOAD_IMAGE_UNCHANGED));
                Metadata meta;
                meta.lat=49.9118449998;meta.lon=-98.26867;meta.time=215410.5+numImage;meta.pitch=-6.8860778809;meta.roll=-10.4903907776;meta.pitchRate=-0.0558875017;meta.rollRate=0.0036246777;meta.yawRate=0.0214070547;meta.altitude=-1.375;meta.heading=175;
                Camera cam = Camera::TestCamera();
                Frame* f = new Frame(img, current_file, meta,cam);

                //Generate Contour
                ObjectDetector detector(filter, ccreator);
                detector.process_frame(f);
                vector<Point> results;
                for (PixelObject * o : f->get_objects()) {
                                       
                    vector<cv::Point> contour = results;//{cv::Point(0,0),cv::Point(0,1),cv::Point(1,1),cv::Point(1,0)};
                    /*
                    cv::Point2d centroid(0,0);
                    double area = 0; 
                    double perimeter = 0;
                    cv::Scalar colour(127,127,127);
                    cv::Point2d error(0,0);
                    double errorAngle = 0;
                    */
                    //BOOST_LOG_TRIVIAL(trace) << "Centroid: " << o->get_centroid();
                    
                    //POList
                    pointerList[numPixelObjects++] = o; //new PixelObject(img,contour,centroid,area,perimeter,colour,error,errorAngle);
                }
                numImage++;
            }

        }
    }
    BOOST_LOG_TRIVIAL(debug) << "Images in test: " << numImage;
    BOOST_LOG_TRIVIAL(debug) << "PixelObjects in test: " << numPixelObjects;

    BOOST_WARN_MESSAGE(numPixelObjects == 75, "Unexpected number of pixel objects, has the target id algorithm changed? Are there more images than expected?"); //If the number of test photos has changed, this integration test may become invalid
    BOOST_WARN_MESSAGE(numImage == 24, "Image repository has been updated. Expect inconsistent test data.");


    //Inject into analysis
    TargetAnalyzer* ta = TargetAnalyzer::getInstance();
    for (int i = 0; i < numPixelObjects; i++){
        BOOST_LOG_TRIVIAL(trace) << "Processing Image: " << i;
        ta->analyze_pixelobject(pointerList[i]); 
    }


    //NEGATIVE CHECK - Check how many different targets there are
    int listLength = ta->get_unique_objects_length();
    BOOST_CHECK(listLength == 42);

    //POSITIVE CHECK - Check how many targets got grouped together and how many
    //in each according to the predefined images => There should  be 3 grouped
    //into a single object.
    vector<Object*> list = ta->extract_objects();
    int i = 0;
    for (Object* o : list){
        int size = o->get_pobjects().size();
        BOOST_LOG_TRIVIAL(debug) << i << ": " << size;
        if (size > 1){
            //This is a little silly, something better should be made.
            BOOST_CHECK_MESSAGE(size <= 7, "Unexpected size");
        }
        i++;
    }
}
