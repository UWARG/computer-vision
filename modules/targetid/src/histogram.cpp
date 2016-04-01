#include "histogram.h"
using namespace std;
using namespace cv;

void target_range::input_b(int a){
    blue.push_back(a);
}

void target_range::input_g(int a){
    green.push_back(a);
}

void target_range::input_r(int a){
    red.push_back(a);
}

bool target_range::belong(Vec3b c){
    for(int i=0;i<blue.size();i++){
        if(blue.at(i)==c.val[0]){
            for(int j=0;j<green.size();j++){
                if(green.at(j)==c.val[1]){
                    for(int k=0;k<red.size();k++){
                        if(red.at(k)==c.val[2])
                            return true;
                    }
                }
             }
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
    int histSize = 256;
    vector<Mat> bgr_planes;
    split(src,bgr_planes);
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    Mat* b_hist=new Mat;
    Mat* g_hist=new Mat;
    Mat* r_hist=new Mat;
    calcHist( &bgr_planes[0], 1, 0, Mat(), *b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), *g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), *r_hist, 1, &histSize, &histRange, uniform, accumulate ); 
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    normalize(*b_hist, *b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(*g_hist, *g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(*r_hist, *r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() ); 
    for( int i = 0; i < histSize; i++ ){
        avg_b[i]=(avg_b[i]*count+cvRound(b_hist->at<float>(i)))/(count+1);
        avg_g[i]=(avg_g[i]*count+cvRound(g_hist->at<float>(i)))/(count+1);
        avg_r[i]=(avg_r[i]*count+cvRound(r_hist->at<float>(i)))/(count+1);
    }
    count++;
    int tmp;
    int minimal=300;
    target_range* pack=new target_range;
    for( int i = 0; i < histSize; i++ ){
        tmp=cvRound(b_hist->at<float>(i)-avg_b[i]);
        if(tmp>minimal){
            pack->input_b(i);
        }
        tmp=cvRound(g_hist->at<float>(i)-avg_g[i]);
        if(tmp>minimal){
            pack->input_g(i);
        }
        tmp=cvRound(r_hist->at<float>(i)-avg_r[i]);
        if(tmp>minimal){
            pack->input_r(i);
        }
    }
    Mat* result=new Mat(src);
    for(int y=0;y<result->rows;y++){
        for(int x=0;x<result->cols;x++){
            Vec3b colour = result->at<Vec3b>(Point(x, y));
            if(!pack->belong(colour)){
                result->at<Vec3b>(Point(x, y)).val[0]=0;
                result->at<Vec3b>(Point(x, y)).val[1]=0;
                result->at<Vec3b>(Point(x, y)).val[2]=0;
            }
        }
    }
    return result;
}

