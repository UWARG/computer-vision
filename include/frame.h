#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

// ----------------------------------------------------------------------------
/**
    File: frame.h
  
    System:         Post Processing
    Component Name: Frame
    Language: C++
  
    (c) Copyright UWARG 2015
  
    Description: Container class for storing photos or frames of video to be
                processed and analyzed
  
**/
// ----------------------------------------------------------------------------

#include <opencv2/core/core.hpp>
#include "target.h"
#include <vector>

using cv::Mat;

class Frame{
public:
    Frame(Mat img, string id, Metadata m);
    string getId();
    Mat get_img(); 
    void add_target(Target t);
    iterator get_targets();
private:
    Mat img;
    string id;
    Metadata data;
    vector<Target> targets;
};


#endif // FRAME_H_INCLUDED
