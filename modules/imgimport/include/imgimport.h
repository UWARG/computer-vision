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
	
#ifndef IMAGE_IMPORT_H_INCLUDED
#define IMAGE_IMPORT_H_INCLUDED

/** 
 * @file imgimport.h
 *
 * @brief Module for matching frames with telemetry information that 
 *        corresponds to them
 *
 * @section Notes   
 *
 * Ideally this should use both the timestamp in the image's
 * exif metadata and the time that the plane records sending
 * the signal to take a picture plus whatever delay that
 * exists between when the signal is sent to the camera and
 * when the camera takes the photograph.
 *
 */

#include "frame.h"
#include <string>

class ImageImport {
    public:
        /**
         * @brief Creates a ImageImport for the telemetry file at the given path
         *
         * @param path Path of the telemetry file
         */
        ImageImport(std::string path);

        ~ImageImport();

        /**
         * @brief Fills in the given frame's metadata structure
         *
         * @param f Frame to be analysed
         */
        Frame * next_frame();
};

#endif // IMAGE_IMPORT_H_INCLUDED
