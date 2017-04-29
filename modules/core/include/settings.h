/**
 * @file settings.h
 * @author WARG
 *
 * @brief Singleton structure which contains settings for each module. These are
 * provided to each module as a reference. As a result, any changes to the
 * values are reflected in the parameters used in each module, live.
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3
 *  clause license
 *  that should have been included with this software in a file called
 *  COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */
	
#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <unordered_map>
#include <string>

union SettingTypes{
    double t_double;
    
};

typedef struct _mSettings{
    std::unordered_map<std::string,SettingTypes> ImageImportSettings;
    std::unordered_map<std::string,SettingTypes> TargetAnalyzerSettings;
    std::unordered_map<std::string,SettingTypes> TargetIdentificationSettings;
} mSettings;

class Settings{
private:

mSettings mset;
static Settings* set;
/*
 * Private constructor to prevent repeated initialization of singleton.
 */
Settings();
public:
    /*
     * getInstance() returns the singleton instance of this class. If it
     * is not
     * instantiated it is initialized.
     * @return the singleton Settings
     */
     static Settings * getInstance();

     mSettings& get_settings();
};

#endif // SETTINGS_H_INCLUDED
