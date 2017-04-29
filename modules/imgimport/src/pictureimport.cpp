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
#include <exiv2/exiv2.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace cv;
using namespace std;
using namespace boost;
namespace bt = boost::posix_time;

static std::time_t pt_to_time_t(const bt::ptime& pt) {
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;
}

static const std::locale dateFormat = std::locale(std::locale::classic(),new bt::time_input_facet("%Y:%m:%d %H:%M:%S"));

PictureImport::PictureImport(std::string filePath, MetadataInput* mdin, Camera &camera)
              :ImageImport(mdin, camera) {
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
    string timestamp;
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
        BOOST_LOG_TRIVIAL(info) << "Reading " << true_path;
        *img=imread(true_path,CV_LOAD_IMAGE_COLOR);
        // TODO: Read timestamp from file exif creation date
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(true_path);
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();
        timestamp = exifData["Exif.Photo.DateTimeOriginal"].value().toString();
    }
    string id(drnt->d_name);
    Metadata m;
    if (reader != NULL) {
        try {
            bt::ptime pt;
            std::istringstream is(timestamp);
            is.imbue(dateFormat);
            is >> pt;
            time_t frameTime = pt_to_time_t(pt);
            tm * utcTime = gmtime(&frameTime);

            double time = utcTime->tm_hour * 10000 + utcTime->tm_min * 100 + utcTime->tm_sec;

            BOOST_LOG_TRIVIAL(trace) << "Picture creation date: " << time;

            m = reader->get_metadata(time);
        } catch (std::exception & e) {
            BOOST_LOG_TRIVIAL(error) << "Error while retrieving metadata: " <<  e.what();
        }
    }
    return new Frame(img, id, m, camera);
}

std::string PictureImport::to_string() {
    return "PictureImport";
}
