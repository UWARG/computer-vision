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
	
#ifndef TARGET_IDENTIFIER_H_INCLUDED
#define TARGET_IDENTIFIER_H_INCLUDED

/**
 * @file target_identifier.h
 *
 * @brief Module for identifying targets in Frames
 *
 * Module analyzes frames using OpenCV tools,
 * locates objects of interest and gathers information 
 * about the objects such as target colour, pixel area,
 * perimeter and shape.
 *
 * The responsibility for deleting Frames and the PixelTargets created
 * by the TargetIdentifier passes to whoever invokes process_frame;
**/

#include "frame.h"
#include "object_detector.h"

class TargetIdentifier{
    ObjectDetector * detector;
    public:
        TargetIdentifier();

        ~TargetIdentifier();


        /**
         * @brieif Processes frame and identifies targets
         *
         * @param f Frame to be processed
         */
        void process_frame(Frame * f);
};


#endif // TARGET_IDENTIFIER_H_INCLUDED
