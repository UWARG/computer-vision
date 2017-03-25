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

#include <opencv2/highgui.hpp>
#include <exiv2/exiv2.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <iomanip>
#include "frame.h"
#include "pixel_object.h"

Frame::Frame(cv::Mat * img, std::string id, Metadata m, Camera &camera)
    : img(camera.undistort(*img)), origImg(img), id(id), data(m), camera(camera) {

}

Frame::~Frame() {
    delete img;
    delete origImg;
}

std::string Frame::get_id(){
    return id;
}

cv::Mat & Frame::get_img(){
    return *img;
}

void Frame::add_object(PixelObject * o){
    this->objects.push_back(o);
    o->image = this;
}

std::vector<PixelObject *> & Frame::get_objects(){
    return objects;
}

const Metadata * Frame::get_metadata(){
    return &data;
}

void Frame::save(std::string dir) {
    std::string path = dir + "/" + id;
    imwrite(path, *img);
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path);
    image->readMetadata();
    Exiv2::ExifData &exifData = image->exifData();
    std::stringstream ss;
    ss << std::setprecision(12) << data.lat << " " << data.lon << " " << data.altitude << " " << data.heading << " " << data.time;
    exifData["Exif.Photo.UserComment"] = ss.str();
    image->writeMetadata();
}
