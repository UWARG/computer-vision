/* 
    This file is part of WARG's computer-vision
    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code 
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used 
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

/**
 * @file frame.h
 *
 * @brief Container class for storing photos or frames of video to be
 *        processed and analyzed
 *
 */

#include <opencv2/core/core.hpp>
#include "metadata.h"
#include <vector>

class Target;
class PixelTarget;

class Frame{
public:
    Frame(cv::Mat * img, std::string id, Metadata m);
    
    /**
     * @brief 
     *
     * @return timestamp identifier for the frame
     */
    std::string get_id();

    /**
     * @brief 
     *
     * @return image associated with the frame
     */
    cv::Mat & get_img(); 

    /**
     * @brief Adds given target to the list of targets 
     *
     * @param t target identified as being in the frame
     */
    void add_target(PixelTarget * t);

    /**
     * @brief getter for targets
     *
     * @return iterator pointing at the first target in the target list
     */
    std::vector<Target*>::iterator get_targets();

    /** 
     * @brief getter for metadata
     *
     * @return const pointer to internal metadata structure
     */
    const Metadata * get_metadata();

private:
    
    /**
     * @brief image associated with the Frame 
     */
    cv::Mat * img;

    /**
     * @brief Timestamp for the frame
     */
    std::string id;

    /**
     * @brief Data collected from telemetry about the plane when the image was captured
     */
    Metadata data;

    /**
     * @brief Targets that appear in the frame
     */
    std::vector<Target *> targets;
};


#endif // FRAME_H_INCLUDED
