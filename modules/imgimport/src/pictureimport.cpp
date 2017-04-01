/**
 * @file picture_import.h
 * @author WARG
 *
 * @section LICENSE
 *
 * Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 * All rights reserved.
 *
 * This software is licensed under a modified version of the BSD 3 clause license
 * that should have been included with this software in a file called COPYING.txt
 * Otherwise it is available at:
 * https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "pictureimport.h"
#include <vector>
#include <string>
#include <dirent.h>
#include <boost/log/trivial.hpp>
#include <iostream>

using namespace cv;
using namespace std;
using namespace boost;

PictureImport::PictureImport(std::string filePath, MetadataInput* mdin)
              :ImageImport(mdin) {
    this->filePath=filePath;
    dr=opendir(filePath.c_str());
}

PictureImport::~PictureImport(){
    closedir(dr);
    BOOST_LOG_TRIVIAL(trace)<<"image import ends."<<endl;
}

Frame * PictureImport::next_frame(){
    Mat* img=new Mat;
    struct dirent* drnt;
    while(img->empty()){
	drnt=readdir(dr);
        if(drnt==NULL){
            BOOST_LOG_TRIVIAL(trace)<<"no more images"<<endl;
            return NULL;
        }
        if(strcmp(drnt->d_name,"..")==0||strcmp(drnt->d_name,".")==0){
            continue;
        }
        BOOST_LOG_TRIVIAL(info) << "Reading " << filePath;
        string true_path=filePath+'/'+drnt->d_name;
        *img=imread(true_path,CV_LOAD_IMAGE_COLOR);
    }
    string id(drnt->d_name);
    Metadata m;
    try {
        m = reader->next_metadata();
    } catch (std::exception & e) {
        BOOST_LOG_TRIVIAL(error) << "Error while retrieving metadata: " <<  e.what();
    }
    return new Frame(img, id, m);
}
