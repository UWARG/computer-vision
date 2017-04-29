/**
 * @file settings.cpp
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

#include <stddef.h>
#include "settings.h"

Settings *Settings::set = NULL;

Settings::Settings(){
//Note: This constructor is private and is only used for initialization.
//TODO: This should read from a JSON file eventually
    mset.TargetAnalyzerSettings.at("COMPARE_AREA").t_double = 400;
    mset.TargetAnalyzerSettings.at("MATCH_THRESHOLD").t_double = 0.5;
    mset.TargetAnalyzerSettings.at("GPS_THRESHOLD").t_double = 0.1;
    mset.TargetAnalyzerSettings.at("VISUAL_THRESHOLD").t_double= 0.6;
    mset.TargetAnalyzerSettings.at("COLOUR_THRESHOLD").t_double= 0.9;
    mset.TargetAnalyzerSettings.at("GPS_THRESHOLD_BIAS").t_double= 0.5;
    mset.TargetAnalyzerSettings.at("VISUAL_THRESHOLD_BIAS").t_double= 0.1;
    mset.TargetAnalyzerSettings.at("COLOUR_THRESHOLD_BIAS").t_double= 0;
}

Settings * Settings::getInstance() {
    if (!set){
        set = new Settings;
    }
    return set;
}

mSettings& Settings::get_settings(){
    return mset;
}
