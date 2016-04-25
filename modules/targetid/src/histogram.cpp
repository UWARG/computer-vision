#include "histogram.h"
using namespace std;
using namespace cv;

HistFilter::HistFilter()
            :Filter(){
}

HistFilter::~HistFilter(){
}

Mat* HistFilter::filter(const Mat & src){
    Mat bgr_img=src.clone();
    int brightness;
    for(int y=0;y<bgr_img.rows;y++){
        for(int x=0;x<bgr_img.cols;x++){
            Vec3b colour = bgr_img.at<Vec3b>(Point(x, y));
            brightness+=colour.val[0];
            brightness+=colour.val[1];
            brightness+=colour.val[2];
        }
    }
    brightness/=(bgr_img.rows*bgr_img.cols*3);
    if(!imgs.count(src.data)) avg_brightness=(avg_brightness*imgs.size()+brightness)/(imgs.size()+1);
    for(int y=0;y<bgr_img.rows;y++){
        for(int x=0;x<bgr_img.cols;x++){
            bgr_img.at<Vec3b>(Point(x, y)).val[0]=bgr_img.at<Vec3b>(Point(x, y)).val[0]+avg_brightness-brightness;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[0]>255) bgr_img.at<Vec3b>(Point(x, y)).val[0]=255;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[0]<0) bgr_img.at<Vec3b>(Point(x, y)).val[0]=0;
            bgr_img.at<Vec3b>(Point(x, y)).val[1]=bgr_img.at<Vec3b>(Point(x, y)).val[1]+avg_brightness-brightness;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[1]>255) bgr_img.at<Vec3b>(Point(x, y)).val[1]=255;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[1]<0) bgr_img.at<Vec3b>(Point(x, y)).val[1]=0;
            bgr_img.at<Vec3b>(Point(x, y)).val[2]=bgr_img.at<Vec3b>(Point(x, y)).val[2]+avg_brightness-brightness;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[2]>255) bgr_img.at<Vec3b>(Point(x, y)).val[2]=255;
            if(bgr_img.at<Vec3b>(Point(x, y)).val[2]<0) bgr_img.at<Vec3b>(Point(x, y)).val[2]=0;
        }
    }
    Mat* hsv_img=new Mat;
    cvtColor(bgr_img,*hsv_img,CV_BGR2HSV);
    blur(*hsv_img,*hsv_img,Size(3,3));
    int hueSize = 18;
    int satSize=16;
    vector<Mat> bgr_planes;
    split(*hsv_img,bgr_planes);
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
    Mat* result=new Mat;
    cvtColor(*hsv_img,*result,CV_HSV2BGR);
    for(int y=0;y<hsv_img->rows;y++){
        for(int x=0;x<hsv_img->cols;x++){
            Vec3b colour = hsv_img->at<Vec3b>(Point(x, y));
            result->at<Vec3b>(Point(x, y)).val[0]=result->at<Vec3b>(Point(x, y)).val[0]*hue_multi[colour.val[0]/10]*colour.val[1]/256;
            result->at<Vec3b>(Point(x, y)).val[1]=result->at<Vec3b>(Point(x, y)).val[1]*hue_multi[colour.val[0]/10]*colour.val[1]/256;
            result->at<Vec3b>(Point(x, y)).val[2]=result->at<Vec3b>(Point(x, y)).val[2]*hue_multi[colour.val[0]/10]*colour.val[1]/256;
        }
    }
    return result;
}

