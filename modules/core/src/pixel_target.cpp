#include "pixel_target.h"

using namespace cv;
using std::string;

PixelTarget::PixelTarget(string type, Point2d centroid, double area, double perimeter, Scalar colour, Point2d error, double errorAngle){
    
}

std::string PixelTarget::get_type(){
    return type;
}

cv::Point2d PixelTarget::get_centroid(){
    return centroid;
}

double PixelTarget::get_area(){
    return area;
}

double PixelTarget::get_perimeter(){
    return perimeter;
}

cv::Scalar PixelTarget::get_colour(){
    return colour;
}

cv::Point2d PixelTarget::get_error(){
    return error;
}

double PixelTarget::get_error_angle(){
    return errorAngle;
}

Frame * PixelTarget::get_image(){
    return image;
}
