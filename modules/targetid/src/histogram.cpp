#include "histogram.h"
using namespace std;
using namespace cv;

void target_range::input_hue(int a){
    hue.push_back(a);
}

bool target_range::belong(Vec3b c){
    for(int i=0;i<hue.size();i++){
        if(hue.at(i)==c.val[0]){
            return true;
        }
    }
    return false;
}

hist_filter::hist_filter()
            :Filter(){
}

hist_filter::~hist_filter(){
}

Mat* hist_filter::filter(const Mat & src){
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
    avg_brightness=(avg_brightness*count+brightness)/(count+1);
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
    float sat_range[]={0,256};
    const float* satRange={sat_range};
    bool uniform = true; bool accumulate = false;
    Mat hue_hist;
    Mat sat_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), hue_hist, 1, &hueSize, &hueRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), sat_hist, 1, &satSize, &satRange, uniform, accumulate );
    for( int i = 0; i < hueSize; i++ ){
        avg_hue[i]=(avg_hue[i]*count+cvRound(hue_hist.at<float>(i)))/(count+1);
    }
    for( int i = 0; i < satSize; i++ ){
        avg_sat[i]=(avg_sat[i]*count+cvRound(sat_hist.at<float>(i)))/(count+1);
    }
    count++;
    int tmp;
    for( int i = 0; i < hueSize; i++ ){
        tmp=cvRound(hue_hist.at<float>(i)-avg_hue[i]);
        if(tmp<0) tmp=0;
        hue_multi[i]=pow(tmp/hue_hist.at<float>(i),5);
    }
    for( int i = 0; i < satSize; i++ ){
        tmp=cvRound(sat_hist.at<float>(i)-avg_sat[i]);
        if(tmp<0) tmp=0;
        sat_multi[i]=pow(tmp/sat_hist.at<float>(i),7);
    }
    Mat* result=new Mat;
    cvtColor(*hsv_img,*result,CV_HSV2BGR);
    for(int y=0;y<hsv_img->rows;y++){
        for(int x=0;x<hsv_img->cols;x++){
            Vec3b colour = hsv_img->at<Vec3b>(Point(x, y));
            result->at<Vec3b>(Point(x, y)).val[0]=10*result->at<Vec3b>(Point(x, y)).val[0]*hue_multi[colour.val[0]/10]*sat_multi[colour.val[1]/16];
            result->at<Vec3b>(Point(x, y)).val[1]=10*result->at<Vec3b>(Point(x, y)).val[1]*hue_multi[colour.val[0]/10]*sat_multi[colour.val[1]/16];
            result->at<Vec3b>(Point(x, y)).val[2]=10*result->at<Vec3b>(Point(x, y)).val[2]*hue_multi[colour.val[0]/10]*sat_multi[colour.val[1]/16];
        }
    }
    return result;
}

