#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

// ----------------------------------------------------------------------------
/**
    File: target.h
  
   System:         Post Processing
   Component Name: Target
   Language: C++
  
   (c) Copyright UWARG 2015
  
   Description: Container class for storing information about
                targets located in photos or video frames
  
**/
// ----------------------------------------------------------------------------

#include <opencv2/core/core.hpp>
#include <vector>

using cv::Mat, cv::Scalar, cv::Point2f, cv::Point;

class Target{
public:
    Target(Mat src, string type);
    string get_type();
    Point2f get_centroid();
    double get_area();
    double get_perimeter();
    Scalar get_colour();
    Point2f get_error();
    double get_error_angle();
private:
    string type;
    vector<Point> contour;
    Point2f centroid;
    double area;
    double perimeter;
    Scalar colour;
    Point2f error;
    double error-angle;
};


#endif // TARGET_H_INCLUDED
