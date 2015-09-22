#include "object_detector.h"
#include "pixel_target.h"
#include <vector>
#include "frame.h"

using namespace std;
using namespace cv;

ObjectDetector::ObjectDetector(Filter * filter, ContourCreator * contourCreator){
    this->filter = filter;
    this->ccreator = contourCreator;
}

void ObjectDetector::process_frame(Frame * f){
    Mat & src = f->get_img();
    Mat * filtered = filter->filter(src);
    vector<vector<Point> > contours = *(ccreator->get_contours(*filtered));
    delete filtered;
    for(vector<Point> contour : contours){
        string type;
        Point2d centroid;
        double area;
        double perimeter;
        Scalar colour;
        Point2d error;
        double errorAngle;

        // get info from contours/image

        PixelTarget * p = new PixelTarget(type, centroid, area, perimeter, colour, error, errorAngle);
        f->add_target(p);
    }
}

ObjectDetector::~ObjectDetector(){

}
