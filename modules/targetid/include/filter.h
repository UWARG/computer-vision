#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <opencv2/core/core.hpp>

class Filter {
    public:
        virtual cv::Mat * filter(const cv::Mat & src) = 0;
};

#endif // FILTER_H_INCLUDED
