#ifndef CANNY_CONTOUR_CREATOR_H_INCLUDED
#define CANNY_CONTOUR_CREATOR_H_INCLUDED

#include "contour_creator.h"

class CannyContourCreator : public ContourCreator {
    public:
        virtual std::vector<std::vector<cv::Point> > * get_contours(cv::Mat & src) = 0;
};

#endif // CANNY_CONTOUR_CREATOR_INCLUDED
