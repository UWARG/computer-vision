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
#include <iostream>
#include "settings.h"

using namespace std;

Settings *Settings::set = NULL;

Settings::Settings(){
//Note: This constructor is private and is only used for initialization.
//TODO: This should read from a JSON file eventually
    mset.TargetAnalyzerSettings.insert({"COMPARE_AREA",400});
    mset.TargetAnalyzerSettings.insert({"MATCH_THRESHOLD",0.5});
    mset.TargetAnalyzerSettings.insert({"GPS_THRESHOLD",0.1});
    mset.TargetAnalyzerSettings.insert({"VISUAL_THRESHOLD",0.6});
    mset.TargetAnalyzerSettings.insert({"COLOUR_THRESHOLD",0.9});
    mset.TargetAnalyzerSettings.insert({"GPS_THRESHOLD_BIAS",0.5});
    mset.TargetAnalyzerSettings.insert({"VISUAL_TRESHOLD_BIAS",0.1});
    mset.TargetAnalyzerSettings.insert({"COLOUR_THRESHOLD_BIAS",0});
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

void Settings::print_settings(){
    for (auto it:mset.ImageImportSettings){
        cout << it.first << ": " << it.second.t_double << endl;
    }
    for (auto it:mset.TargetAnalyzerSettings){
        cout << it.first << ": " << it.second.t_double << endl;
    }
    for (auto it:mset.TargetIdentificationSettings){
        cout << it.first << ": " << it.second.t_double << endl;
    }
}
