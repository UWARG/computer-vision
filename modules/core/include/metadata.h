/* 
    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code must be explicitly referenced to WARG and this code 
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used 
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
