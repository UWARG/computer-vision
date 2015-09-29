/*This file is part of WARG's computer-vision.

    computer-vision is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. You cannot however, claim it as your own. 
	Any code used MUST be referenced to WARG (Waterloo Aerial Robotics Group). 
	Additionally, this code cannot be used in any competition against WARG.

    computer-vision is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with computer-vision.  If not, see <http://www.gnu.org/licenses/>.
	*/
	
#ifndef METADATA_H_INCLUDED
#define METADATA_H_INCLUDED

/**
 * @file metadata.h
 *
 * @brief Structure for storing plane status information from the time
 *        that an image was taken
 *
 */

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
