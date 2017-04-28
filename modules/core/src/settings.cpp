/**
 * @file settings.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
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

Settings * Settings::getInstance() {
    if (!set){
        set = new Settings;
    }
    return set;
}

mSettings& Settings::get_settings(){
    return mset;
}
