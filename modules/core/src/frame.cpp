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

#include "frame.h"

Frame::Frame(cv::Mat * img, std::string id, Metadata m): img(img), id(id), data(m){

}
    
std::string Frame::get_id(){
    return id;
}

cv::Mat & Frame::get_img(){
    return *img;
}

void Frame::add_target(PixelTarget * t){

}

std::vector<Target*>::iterator Frame::get_targets(){
    return targets.begin();
}

const Metadata * Frame::get_metadata(){
    return &data;
}

void Frame::set_img(cv::Mat* img){
    this->img=img;
}

void Frame::set_id(std::string id){
    this->id=id;
}

void Frame::set_m(Metadata m){
    data.time=m.time;
    data.timeError=m.timeError;
    data.lat=m.lat;
    data.lon=m.lon;
    data.latError=m.latError;
    data.lonError=m.lonError;
    data.pitch=m.pitch;
    data.roll=m.roll;
    data.pitchRate=m.pitchRate;
    data.rollRate=m.rollRate;
    data.yawRate=m.yawRate;
    data.altitude=m.altitude;
    data.heading=m.heading;
}
