#include "histogram.h"
using namespace std;
using namespace cv;
typedef cv::Point3_<uint8_t> Pixel;

HistFilter::HistFilter()
            :Filter(){
}

HistFilter::~HistFilter(){
}

Mat* HistFilter::filter(const Mat & src){
    Mat hsv_img;
    cvtColor(src,hsv_img,CV_BGR2HSV);
    blur(hsv_img,hsv_img,Size(3,3));
    int hueSize = 18;
    int satSize=16;
    vector<Mat> bgr_planes;
    split(hsv_img,bgr_planes);
    float hue_range[]={0,180};
    const float* hueRange={hue_range};
    bool uniform = true; bool accumulate = false;
    Mat hue_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), hue_hist, 1, &hueSize, &hueRange, uniform, accumulate );
    if (!imgs.count(src.data)) {
        for( int i = 0; i < hueSize; i++ ){
            avg_hue[i]=(avg_hue[i]*imgs.size()+cvRound(hue_hist.at<float>(i)))/(imgs.size()+1);
        }
        imgs.insert(src.data);
    }
    int tmp;
    for( int i = 0; i < hueSize; i++ ){
        tmp=cvRound(hue_hist.at<float>(i)-avg_hue[i]);
        if(tmp<0) tmp=0;
        hue_multi[i]=pow(tmp/hue_hist.at<float>(i),2);
    }
    Mat p = src.clone();
    blur(p,p,Size(3,3));
    /*for(int y=0;y<hsv_img.rows;y++){
        for(int x=0;x<hsv_img.cols;x++){
            Vec3b colour = hsv_img.at<Vec3b>(y * hsv_img.cols + x);
            p(x, y)[0] *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
            p(x, y)[1] *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
            p(x, y)[2] *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
        }
    }*/
    p.forEach<Pixel>([&](Pixel &p, const int * position) -> void {
            Vec3b colour = hsv_img.at<Vec3b>(position[0] * hsv_img.cols + position[1]);
            p.y *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
            p.x *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
            p.z *= hue_multi[colour.val[0]/10]*colour.val[1]/256;
    });
    return new Mat(p);
}

