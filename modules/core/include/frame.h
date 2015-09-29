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
    Frame(cv::Mat img, std::string id, Metadata m);
    
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
    cv::Mat img;

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
