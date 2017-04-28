/**
 * @file object.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */


#include "object.h"
#include "pixel_object.h"

Object::Object(){

}

void Object::add_pobject(PixelObject * po){
    pixelObjects.push_back(po);
    
    //Update the Object parameters
    int n = Object::pixelObjects.size();
    if (n > 1){
        double ratio = (double)n/(n+1);
        cv::Point2d maxDistance = this->error + this->centroid;
        if (sqrt(pow(maxDistance.x,2) + pow(maxDistance.y,2)) < sqrt(pow(po->get_gps_centroid().x,2) + pow(po->get_gps_centroid().y,2))){
            maxDistance = po->get_gps_centroid();
        }
    
        this->centroid = this->centroid*ratio + po->get_gps_centroid()/((double)n+1);
        this->area = this->area*ratio + po->get_gps_area()/((double)n+1);
        this->colour = this->colour*ratio + po->get_colour()/((double)n+1);

    
        //TODO: Determine image quality, choose best image
        this->error = maxDistance - this->centroid;
        this->errorAngle = atan2(this->centroid.y - maxDistance.y, this->centroid.x - maxDistance.x);
    }
    else{
        recalculate();
    }
}

cv::Point2d Object::get_centroid(){
    return centroid;
}

cv::Scalar Object::get_colour(){
    return colour;
}


void Object::recalculate(){
    int n = Object::pixelObjects.size();
    this->centroid = cv::Point2d(0,0);
    cv::Point2d maxDistance(0,0);
    cv::Scalar cSum;
    for (PixelObject* po : pixelObjects){
        this->centroid += po->get_gps_centroid();
        this->area += po->get_gps_area();
        cSum += po->get_colour();

        if (sqrt(pow(maxDistance.x,2) + pow(maxDistance.y,2)) <
        sqrt(pow(po->get_gps_centroid().x,2) + pow(po->get_gps_centroid().y,2))){
            maxDistance = po->get_gps_centroid();
        }
        //TODO: Determine image quality, choose best image

    }
    this->centroid /= n;
    this->colour = cSum/n;
    this->area /= n;
    this->error = maxDistance - this->centroid;
    this->errorAngle = atan2(this->centroid.y - maxDistance.y, this->centroid.x - maxDistance.x);
}

const std::vector<PixelObject*>& Object::get_pobjects(){
    return pixelObjects; 
}


