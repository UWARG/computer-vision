#include "frame.h"

Frame::Frame(cv::Mat img, std::string id, Metadata m){

}
    
std::string Frame::get_id(){
    return id;
}

cv::Mat & Frame::get_img(){
    return img;
}

void Frame::add_target(PixelTarget * t){

}

std::vector<Target*>::iterator Frame::get_targets(){
    return targets.begin();
}

const Metadata * Frame::get_metadata(){
    return &data;
}
