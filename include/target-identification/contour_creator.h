#ifndef CONTOUR_CREATOR_H_INCLUDED
#define CONTOUR_CREATOR_H_INCLUDED

#include <opencv2/core/core.hpp>

class ContourCreator {
    public:
        virtual std::vector<std::vector<cv::Point> > get_contours(cv::Mat & src) = 0;
};

#endif // CONTOUR_CREATOR_H_INCLUDED
