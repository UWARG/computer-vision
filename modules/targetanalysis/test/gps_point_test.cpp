/**
 * @file gps_point_test.cpp
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

#include "pixel_object_list.h"
#include "target_analyzer.h"
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "frame.h"
#include "metadata_reader.h"
#include "metadata_input.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iomanip>

using namespace std;
using namespace boost;
using namespace cv;

namespace logging = boost::log;
using namespace boost::filesystem;

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


BOOST_AUTO_TEST_CASE(GPSHighAltitudePointTest){
    
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
    PixelObject* pointerList[fileCount];
    KMeansFilter * filter = new KMeansFilter();
    CannyContourCreator * ccreator = new CannyContourCreator();
    
    double longitude, latitude, altitude;

    for ( directory_iterator itr( root_path ); itr != end_itr; ++itr ){
        if ( is_regular_file(itr->path()) ){
            string current_file = itr->path().string();
            if (has_suffix(current_file, ".jpg") || has_suffix(current_file,
            ".jpeg")){
               

                //Manually generate a PixelObject
                MetadataInput* mi = new MetadataInput();
                string filename = root_path + "/../test_csv.csv";
                MetadataReader mr(*mi,filename);
                cv::Mat* img = new cv::Mat(imread(current_file,CV_LOAD_IMAGE_UNCHANGED));
                //Skip first zero row
                mi->next_metadata();
                const Metadata m = mi->get_metadata(220649);
                Camera cam = Camera::TestCamera();
                Frame* f = new Frame(img, current_file, m, cam);
                
                longitude = m.lon;
                latitude = m.lat;
                altitude = m.altitude;
                BOOST_LOG_TRIVIAL(debug) << "Frame->Longitude: " << m.lon <<
                "; Latitude: " << m.lat << "; Altitude: " << m.altitude << endl; 
                                
                //Generate Contour
                ObjectDetector detector(filter, ccreator);
                detector.process_frame(f);
                vector<Point> results;
                PixelObject* o;
                for (PixelObject * o : f->get_objects()) {
                                       
                    vector<cv::Point> contour = results;
                                       
                    //POList
                    pointerList[numPixelObjects++] = o;
                }
                numImage++;
                break;
            }

        }
    }
       
    //Inject into GPS Location Function
    PixelObjectList* po = PixelObjectList::getInstance();
    cv::Point2d cameraAlpha(69.5/2,118.2/2);
    cv::Point2d* geoLocation[numPixelObjects];
    for (int i = 0; i < numPixelObjects; i++){
        BOOST_LOG_TRIVIAL(trace) << "Processing PixelObject: " << i;
        geoLocation[i] = new cv::Point2d();
        TargetAnalyzer* ta = TargetAnalyzer::getInstance();
        BOOST_CHECK(ta->getGPS(pointerList[i]->get_centroid(),cameraAlpha,pointerList[i]->get_image(),geoLocation[i])==1);   
        double gpsDistance = ta->getGPSDistance(latitude, longitude,
        geoLocation[i]->x, geoLocation[i]->y);
 
        double dLat = abs(latitude - geoLocation[i]->x);
        double dLon = abs(longitude - geoLocation[i]->y);
    
        BOOST_CHECK_MESSAGE(dLat > 1e-4 && dLat < 1e-2, "Calculated Latitude is not within acceptable limits: " + to_string(dLat));
        BOOST_CHECK_MESSAGE(dLon > 1e-4 && dLon < 1e-2, "Calculated Longitude is not within acceptable limits: " + to_string(dLon));

        BOOST_CHECK_MESSAGE(gpsDistance < 167 && gpsDistance > 83, "Distance from center GPS is too large or too small.");
        
        BOOST_LOG_TRIVIAL(info) << "New GPS Coordinates: " << *geoLocation[i]; 
        BOOST_LOG_TRIVIAL(info) << "dlat: " << dLat << " dlon: " << dLon;
        BOOST_LOG_TRIVIAL(info) << "Distance from center of photo: " << gpsDistance;
    
    }

    //Cleanup
    delete filter;
    delete ccreator;
    for (int i = 0; i < numPixelObjects; i++){
        delete geoLocation[i];
    }
}

BOOST_AUTO_TEST_CASE(GPSLowAltitudePointTest){
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
    PixelObject* pointerList[fileCount];
    KMeansFilter * filter = new KMeansFilter();
    CannyContourCreator * ccreator = new CannyContourCreator();
    double longitude, latitude, altitude;

    for ( directory_iterator itr( root_path ); itr != end_itr; ++itr ){
        if ( is_regular_file(itr->path()) ){
            string current_file = itr->path().string();
            if (has_suffix(current_file, ".jpg") || has_suffix(current_file,
            ".jpeg")){
               

                //Manually generate a PixelObject
                MetadataInput* mi = new MetadataInput();
                string filename = root_path + "/../test_csv.csv";
                MetadataReader mr(*mi,filename);
                cv::Mat* img = new cv::Mat(imread(current_file,CV_LOAD_IMAGE_UNCHANGED));
                //Skip first zero row
                mi->next_metadata();
                const Metadata m = mi->next_metadata();
                Camera cam = Camera::TestCamera();
                Frame* f = new Frame(img, current_file, m, cam);
                
                longitude = m.lon;
                latitude = m.lat;
                altitude = m.altitude;
                BOOST_LOG_TRIVIAL(debug) << "Frame->Longitude: " << m.lon <<
                "; Latitude: " << m.lat << "; Altitude: " << m.altitude << endl; 
                                
                //Generate Contour
                ObjectDetector detector(filter, ccreator);
                detector.process_frame(f);
                vector<Point> results;
                for (PixelObject * o : f->get_objects()) {
                                       
                    /*vector<cv::Point> contour = o->get_contour();
                    vector<vector<cv::Point>> contourWrapper;
                    contourWrapper.push_back(contour);
                    Mat drawing = Mat::zeros(2000,2000, CV_8UC3);
                    drawContours(drawing,contourWrapper,0,cv::Scalar(255,0,255),FILLED);
                    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
                    imshow("Contours",drawing);
                    waitKey(0);*/

                    //POList
                    pointerList[numPixelObjects++] = o;
                }
                numImage++;
                break;
            }

        }
    }
       
    //Inject into GPS Location Function
    PixelObjectList* po = PixelObjectList::getInstance();
    cv::Point2d cameraAlpha(69.5/2,118.2/2);
    cv::Point2d* geoLocation[numPixelObjects];
    for (int i = 0; i < numPixelObjects; i++){
        BOOST_LOG_TRIVIAL(trace) << "Processing PixelObject: " << i;
        geoLocation[i] = new cv::Point2d();
        TargetAnalyzer* ta = TargetAnalyzer::getInstance();
        BOOST_CHECK(ta->getGPS(pointerList[i]->get_centroid(),cameraAlpha,pointerList[i]->get_image(),geoLocation[i])==1); 
            
        double dLat = abs(latitude - geoLocation[i]->x);
        double dLon = abs(longitude - geoLocation[i]->y);
        double gpsDistance = ta->getGPSDistance(latitude, longitude,
        geoLocation[i]->x, geoLocation[i]->y);

        //Low altitude image should have near zero displacement on imagery
        BOOST_CHECK_MESSAGE(dLat < 1e-4, "Calculated Latitude is not within acceptable limits: " + to_string(dLat));
        BOOST_CHECK_MESSAGE(dLon < 1e-4, "Calculated Longitude is not within acceptable limits: " + to_string(dLon));
        
        BOOST_CHECK_MESSAGE(gpsDistance < 5, "Distance from center GPS is too large.");
        
        BOOST_LOG_TRIVIAL(info) << "New GPS Coordinates: " << *geoLocation[i];
        BOOST_LOG_TRIVIAL(info) << "dlat: " << dLat << " dlon: " << dLon;
        BOOST_LOG_TRIVIAL(info) << "Distance from center of photo: " << gpsDistance;
    
    }
         
    //Cleanup
    delete filter;
    delete ccreator;
    for (int i = 0; i < numPixelObjects; i++){
        delete geoLocation[i];
    }
}
