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

vector<Mat> histogram_recreator(PictureImport* input){
    vector<bgr_hists> original;
    vector<target_range> tr;
    Frame* src;
    vector<Mat> result;
    int histSize = 256;
    double avg_b[histSize],avg_g[histSize],avg_r[histSize];
    for(src=input->next_frame();src!=NULL;src=input->next_frame()){
        vector<Mat> bgr_planes;
        split(src->get_img(),bgr_planes);
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
            avg_b[i]+=cvRound(b_hist->at<float>(i));
            avg_g[i]+=cvRound(g_hist->at<float>(i));
            avg_r[i]+=cvRound(r_hist->at<float>(i));
        }
        bgr_hists* pack=new bgr_hists;
        pack->b_hist=*b_hist;
        pack->g_hist=*g_hist;
        pack->r_hist=*r_hist;
        original.push_back(*pack);
        result.push_back(src->get_img());
    }
    for( int i = 0; i < histSize; i++ ){
        avg_b[i]=cvRound(avg_b[i]/original.size());
        avg_g[i]=cvRound(avg_g[i]/original.size());
        avg_r[i]=cvRound(avg_r[i]/original.size());
    }
    for(int j=0;j<original.size();){
        int tmp;
        int minimal=100;
        bool f=false;
        target_range* pack=new target_range;
        for( int i = 0; i < histSize; i++ ){
            tmp=cvRound(original.at(j).b_hist.at<float>(i)-avg_b[i]);
            if(tmp>minimal){
                f=true;
                pack->input_b(i);
            }
            tmp=cvRound(original.at(j).g_hist.at<float>(i)-avg_g[i]);
            if(tmp>minimal){
                f=true;
                pack->input_g(i);
            }
            tmp=cvRound(original.at(j).r_hist.at<float>(i)-avg_r[i]);
            if(tmp>minimal){
                f=true;
                pack->input_r(i);
            }
        }
        if(f){
            tr.push_back(*pack);
            j++;
        }
        else{
            original.erase(original.begin()+j);
            result.erase(result.begin()+j);
        }
    }
    for(int k=0;k<result.size();k++){
        for(int y=0;y<result.at(k).rows;y++){
            for(int x=0;x<result.at(k).cols;x++){
                Vec3b* colour = &(result.at(k).at<Vec3b>(Point(x, y)));
                if(!tr.at(k).belong(*colour)){
                    colour->val[0]=0;
                    colour->val[1]=0;
                    colour->val[2]=0;
                }
            }
        }
    }
    return result;
}

