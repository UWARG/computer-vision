#include "canny_contour_creator.h"
#include <boost/log/trivial.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using std::vector;
using namespace cv;

const int lowThreshold = 60;
const int ratio = 3;
const int kernelSize = 3;

vector<vector<Point> > * CannyContourCreator::get_contours(cv::Mat & src) {
    Mat result;
    vector<vector<Point> > * contours = new vector<vector<Point> >() ;
    vector<Vec4i> hierarchy;
    /// Canny detector
    Canny( src, result, lowThreshold, lowThreshold*ratio, kernelSize );

    BOOST_LOG_TRIVIAL(info) << "Detecting Contours";
    /// Find contours
    findContours( result, *contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    BOOST_LOG_TRIVIAL(info) << "Found " << contours->size() << " Contours";
    return contours;
}
