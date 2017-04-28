/**
 * @file metadata.h
 * @author WARG
 *
 * @brief Structure for storing plane status information from the time
 *        that an image was taken
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
	
#ifndef METADATA_H_INCLUDED
#define METADATA_H_INCLUDED

struct Metadata{
    /**
     * @brief Time of Frame capture
     */
    double time;

    /**
     * @brief Error associated with the time
     */
    double timeError;

    /**
     * @brief Latitude of the plane at time of frame capture
     */
    double lat;
    
    /**
     * @brief Longitude of the plane at time of frame capture
     */
    double lon;
    
    /**
     * @brief Error associated with the latitude 
     */
    double latError;
    
    /**
     * @brief Error associated with the longitude
     */
    double lonError;

    /**
     * @brief Pitch of the camera at time of frame capture
     */
    double pitch;
    
    /**
     * @brief Roll of the camera at time of frame capture
     */
    double roll;

    /**
     * @brief Pitch rate of the camera at time of frame capture
     */
    double pitchRate;
    
    /**
     * @brief Roll rate of the camera at time of frame capture
     */
    double rollRate;
    
    /**
     * @brief yaw rate of the plane at time of frame capture
     */
    double yawRate;

    /**
     * @brief altitude of the plane at time of frame capture 
     */
    double altitude;
    
    /**
     * @brief heading of the plane at time of frame capture
     */
    double heading;
    
    /**
     * @brief error associated with the plane's altitude
     */
    double altError;
    
    /**
     * @brief Error associated with the plane's heading
     */
    double headingError;
};


#endif // METADATA_H_INCLUDED
