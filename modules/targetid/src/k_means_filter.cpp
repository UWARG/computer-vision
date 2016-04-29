/*
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "k_means_filter.h"
#include <boost/log/trivial.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>

using namespace cv;
typedef cv::Point3_<uint8_t> Pixel;

    typedef cv::Point3_<uint8_t> Pixel;
KMeansFilter::KMeansFilter() {
    (*this->parameters)["reductionFactor"] = 2; //beyond 2 seems to decrease effectiveness of the diff'd image; beyong 1 causes segfaults
    (*this->parameters)["clusters"] = 4;
    (*this->parameters)["attempts"] = 1;
    (*this->parameters)["noiseReduction"] = 5;
}

cv::Mat * KMeansFilter::filter(const Mat & src) {
    int kernelSize = 3;
    int reductionFactor = (*this->parameters)["reductionFactor"];
    BOOST_LOG_TRIVIAL(trace) << "Calculating kmeans...";
    BOOST_LOG_TRIVIAL(debug) << "reductionFactor = " << (*parameters)["reductionFactor"];
    BOOST_LOG_TRIVIAL(debug) << "clusters = " << (*parameters)["clusters"];
    BOOST_LOG_TRIVIAL(debug) << "attempts = " << (*parameters)["attempts"];
    BOOST_LOG_TRIVIAL(debug) << "noiseReduction = " << (*parameters)["noiseReduction"];

    Mat tmp;
    resize(src, tmp, Size(src.cols/reductionFactor, src.rows/reductionFactor));

    // Analyze colour clusters in image
    Mat samples = tmp.reshape(1, tmp.rows * tmp.cols);
    samples.convertTo(samples, CV_32F);

    int clusterCount = (*this->parameters)["clusters"];
    Mat labels;
    int attempts = (*this->parameters)["attempts"];
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::MAX_ITER, 1, 1), attempts, KMEANS_PP_CENTERS, centers );

    BOOST_LOG_TRIVIAL(trace) << "Generating new image...";
    BOOST_LOG_TRIVIAL(debug) << "Src " << src.cols << "x" << src.rows << " tmp " << tmp.cols << "x" << tmp.rows;
    Mat * new_image = new Mat( src.size(), src.type() );
    BOOST_LOG_TRIVIAL(debug) << "new " << new_image->cols << "x" << new_image->rows << " Labels " << labels.rows << " " << labels.cols;

    new_image->forEach<Pixel>([&](Pixel &p, const int * position) -> void {
            int index = position[1]/reductionFactor + (position[0]/reductionFactor) * tmp.cols;
            int clusterIdx = labels.at<int>(index, 0);

            p.x = abs(src.at<Vec3b>(position[0], position[1])[0] - centers.at<float>(clusterIdx, 0));
            p.y = abs(src.at<Vec3b>(position[0], position[1])[1] - centers.at<float>(clusterIdx, 1));
            p.z = abs(src.at<Vec3b>(position[0], position[1])[2] - centers.at<float>(clusterIdx, 2));

        }
    );

    BOOST_LOG_TRIVIAL(trace) << "Reducing Noise...";
    int noiseReduction = (*this->parameters)["noiseReduction"];
    erode(*new_image, *new_image, noiseReduction);
    dilate(*new_image, *new_image, noiseReduction);

    /// Reduce noise with a kernel
    blur(*new_image, *new_image, Size(kernelSize,kernelSize));
    return new_image;
}
