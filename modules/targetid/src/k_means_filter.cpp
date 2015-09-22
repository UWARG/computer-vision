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
    kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER, 1, 1), attempts, KMEANS_PP_CENTERS, centers );

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
