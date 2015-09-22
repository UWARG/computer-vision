#ifndef K_MEANS_FILTER_H_INCLUDED
#define K_MEANS_FILTER_H_INCLUDED

#include "filter.h"

class KMeansFilter : public Filter {
    public:
        cv::Mat * filter(const cv::Mat & src);
};

#endif // K_MEANS_FILTER_H_INCLUDED
