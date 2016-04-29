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
