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

using namespace cv;

cv::Mat * KMeansFilter::filter(const Mat & src) {
    int ratio = 3;
    int kernel_size = 3;
    int reductionFactor = 2; //beyond 2 seems to decrease effectiveness of the diff'd image
    BOOST_LOG_TRIVIAL(info) << "Calculating kmeans...";

    Mat tmp;
    resize(src, tmp, Size( src.cols/reductionFactor, src.rows/reductionFactor ));

    /// Analyze colour clusters in image
    Mat samples(tmp.rows * tmp.cols, 3, CV_32F);
    for( int y = 0; y < tmp.rows; y++ )
        for( int x = 0; x < tmp.cols; x++ )
            for( int z = 0; z < 3; z++)
                samples.at<float>(y + x*tmp.rows, z) = tmp.at<Vec3b>(y,x)[z];

    int clusterCount = 10;
    Mat labels;
    int attempts = 1;
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(TermCriteria::MAX_ITER, 1, 1), attempts, KMEANS_PP_CENTERS, centers );

    BOOST_LOG_TRIVIAL(info) << "Generating new image...";
    Mat * new_image = new Mat( src.size(), src.type() );
    for( int y = 0; y < src.rows; y++ ) {
        for( int x = 0; x < src.cols; x++ ) {
            int cluster_idx = labels.at<int>((y/reductionFactor) + (x/reductionFactor)*(src.rows/reductionFactor),0);
            new_image->at<Vec3b>(y,x)[0] = abs(src.at<Vec3b>(y,x)[0] - centers.at<float>(cluster_idx, 0));
            new_image->at<Vec3b>(y,x)[1] = abs(src.at<Vec3b>(y,x)[1] - centers.at<float>(cluster_idx, 1));
            new_image->at<Vec3b>(y,x)[2] = abs(src.at<Vec3b>(y,x)[2] - centers.at<float>(cluster_idx, 2));
        }
    }
    erode(*new_image, *new_image, 5);
    dilate(*new_image, *new_image, 5);

    /// Reduce noise with a kernel
    blur(*new_image, *new_image, Size(kernel_size,kernel_size));
    return new_image;
}
