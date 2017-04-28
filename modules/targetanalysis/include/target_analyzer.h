/**
 * @file target_analyzer.h
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


#ifndef TARGET_ANALYZER_H_INCLUDED
#define TARGET_ANALYZER_H_INCLUDED

/**
 * @file target_analyzer.h
 *
 * @brief Module for analyzing individual PixelTargets and combining them into 
 *        a set of unique Targets
 *
 * Module geolocates targets using their pixel locations
 * and photo metadata, determines target type and calculates 
 * possible error. As targets are processed unique targets will 
 * be identified and the data combined into a single object.
 *
 * The TargetAnalyzer does not delete any Frames or PixelTargets
 *
**/

#include "frame.h"
#include "pixel_object.h"
#include "object.h"
#include <math.h>
#include <vector>

//Classes

/**
 * @class TargetAnalyzer
 *  
 * TargetAnalyzer is a wrapper module for the PixelObjectList class. It handles
 * the entry and exit points for every form of interaction with the target
 * analysis module.
 * This class is a singleton.
 *
 * @brief Provides a procedure for analyzing pixel objects, as well as
 * extracting data from the results. PixelObjectList is a singleton.
 *
 */
class TargetAnalyzer {
    /*
     * analyzer is the static singleton instance of the TargetAnalyzer class
     */
    static TargetAnalyzer * analyzer;
    /*
     * Private constructor due to singleton design pattern.
     */ 
    TargetAnalyzer(){};

    /*
     * Settings variables for threshold and threshold biasing in the
     * grouping algorithm
     *
     * VISUAL_THRESHOLD determines how similar two contours need to be inorder
     * to be designated a 'duplicate'.
     *
     * GPS_THRESHOLD determines how similar two GPS coordinates must be inorder
     * to be designated a 'duplicate'. A value of 0.1 coorresponds to 10 meter
     * accuracy (1/0.1 = 10m)
     *
     * COLOUR_THRESHOLD determines how similar two pixel object colours must be inorder
     * to be designated a 'duplicate'. This is based on a relative RGB scale,
     * where White and Black are the polar opposites and would be designated a 0
     * (based on a euclidean RGB distance).
     *
     * GPS_THRESHOLD_BIAS determines how important GPS similarity is in
     * comparison to VISUAL and COLOR parameters. This value is applied AFTER it matches
     * the original GPS_THRESHOLD.
     *   
     * VISUAL_THRESHOLD_BIAS determines how important GPS similarity is in
     * comparison to GPS and COLOR parameters. This value is applied AFTER it matches
     * the original VISUAL_THRESHOLD.
     *
     * COLOUR_THRESHOLD_BIAS determines how important colour similarity is in
     * comparison to GPS and visual/physical parameters. This value is applied AFTER it matches
     * the original COLOUR_THRESHOLD.
     */
    double THRESHOLD[4] = {0.1,0.6,0.9,0.5}; double THRESHOLD_BIAS[4] =
    {0.5,0.1,0,0};

    public:
        /*
         * getInstance() returns the singleton instance of this class. If it is not
         * instantiated it is initialized.
         * @return the singleton TargetAnalyzer
         */
        static TargetAnalyzer * getInstance(){
            if (!analyzer){
                analyzer = new TargetAnalyzer;
            }
            return analyzer;
        }
   
        /*
         * @brief AlgorithmNum enumerates a variety of settings for determining
         * the variance and significance of GPS, CONTOUR and COLOUR correlation.
         */
        enum AlgorithmNum{
            GPS = 0,
            CONTOUR = 1,
            COLOUR = 2,
            MATCH = 3,
        };

        /*
         * analyze_pixelobject(PixelObject* po) is the entry function into this
         * module, it initializes the comparisons that are made for each
         * PixelObject.
         * @param po the PixelObject that as being analyzed.
         */
        void analyze_pixelobject(PixelObject* po);

        /*
         * @brief get_threshold() is a getter for the threshold parameter of a
         * particular analysis algorithm
         * @param an the algorithm for which the threshold settings are
         * requested for, as defined by enum AlgorithmNum
         * @return the threshold setting for the particular algorithm
         */
        double get_threshold(AlgorithmNum an);
       
        /*
         * @brief get_threshold_bias() is a getter for the threshold_bias
         * parameter of a particular analysis algorithm.
         * @param an the algorithm for which the threshold_bias settings are
         * requested for, as defined by enum AlgorithmNum
         * @return the threshold_bias setting for the particular algorithm
         */
        double get_threshold_bias(AlgorithmNum an); 
        
        /*
         * @brief set_threshold() is a setter for the threshold parameter of a
         * particular analysis algorithm
         * @param an the algorithm for which the threshold settings are
         * requested for, as defined by enum AlgorithmNum
         * @param value the threshold setting for the particular algorithm
         */
        void set_threshold(AlgorithmNum an, double value);
       
        /*
         * @brief set_threshold_bias() is a getter for the threshold_bias
         * parameter of a particular analysis algorithm.
         * @param an the algorithm for which the threshold_bias settings are
         * requested for, as defined by enum AlgorithmNum
         * @param value the threshold_bias setting for the particular algorithm
         */
        void set_threshold_bias(AlgorithmNum an, double value); 
        /*
         * getGPS(...) calculates the GPS coordinates (latitude, longitude) of a
         * specific point in a frame.
         * @param point the point at which the latitude and longitude should be
         * determined at.
         * @param cameraAlpha the alpha angle of the lens of the camera. This
         * defines how much can be seen and what effect altitude has on the image
         * scaling.
         * @param f the frame for which the point is calculated for.
         * @return A Point, where the first value is the latitude and the second is
         * the longitude.
         */
        int getGPS(cv::Point2d point, cv::Point2d cameraAlpha,Frame* f, cv::Point2d* returnResult);


        /*
         * @brief getGPSDistance() calculates the distance between two GPS coordinates in meters.
         * @param lat1 the latitude of the first point
         * @param lon1 the longitude of the first point
         * @param lat2 the latitude of the second point
         * @param lon2 the longitude of the second point
         * @return the distance in meters between the two GPS coordinates.
         */
        double getGPSDistance(double lat1, double lon1, double lat2, double lon2);

        /*
         * @brief getGPSDistance() calculates the distance between two GPS coordinates in meters.
         * @param gps1 the gps coordinates of the first point
         * @param gps2 the gps coordinates of the second point
         * @return the distance in meters between the two GPS coordinates.
         */
        double getGPSDistance(cv::Point2d gps1, cv::Point2d gps2); 

        /*
         * extract_objects() provides a list of unique objects that have been
         * identified, where the non-unique ones have been grouped.
         * @return the vector of Objects with identifies the unique ones found.
         */
        std::vector<Object*> extract_objects();

        /*
         * get_unique_objects_length() provides the total number of objects that
         * have been found within the data acquired.
         * @return the integer number of Objects that can be extracted by using
         * the extract_objects() function.
         */ 
        int get_unique_objects_length();
};

#endif // TARGET_ANALYZER_H_INCLUDED
