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

typedef struct _TargetAnalyzerSettings{
    /**
     * @brief Area to use for visual comparisons. Larger values require more
     * computational time.
     */
    int COMPARE_AREA;

    /**
     * @brief Error associated with the time
     */
    double MATCH_THRESHOLD;

    /**
     * @brief Latitude of the plane at time of frame capture
     */
    double GPS_THRESHOLD;
    
    /**
     * @brief Longitude of the plane at time of frame capture
     */
    double VISUAL_THRESHOLD;
    
    /**
     * @brief Error associated with the latitude 
     */
    double COLOUR_THRESHOLD;
    
    /**
     * @brief Error associated with the longitude
     */
    double GPS_THRESHOLD_BIAS;

    /**
     * @brief Pitch of the camera at time of frame capture
     */
    double VISUAL_THRESHOLD_BIAS;
    
    /**
     * @brief Roll of the camera at time of frame capture
     */
    double COLOUR_THRESHOLD_BIAS;

} TargetAnalyzerSettings;

typedef struct _ImageImportSettings{
    int a;       
} ImageImportSettings;

typedef struct _TargetIdentificationSettings{
    int a;
} TargetIdentificationSettings;

typedef struct _mSettings{
    ImageImportSettings im;
    TargetAnalyzerSettings ta;
    TargetIdentificationSettings ti;
} mSettings;

class Settings{
private:

mSettings mset;
static Settings* set;
/*
 * Private constructor to prevent repeated initialization of singleton.
 */
Settings(){};
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
