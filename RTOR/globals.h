#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

struct c_data
{
    Scalar average_colour;
    Point2f centre;
    double area;
    int arc_length;
};

bool isWithin(int A, int B, int C);

void log(int level, string message);




#endif // GLOBALS_H_INCLUDED
