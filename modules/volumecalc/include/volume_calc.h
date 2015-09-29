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
	
#ifndef VOLUME_CALC_H_INCLUDED
#define VOLUME_CALC_H_INCLUDED

/**
 * @file volume_calc.h
 * 
 * @brief Module for calculating the volume of a target
 *
 */

#include "frame.h" 
#include "target.h"

class VolumeCalc {
    public:
        VolumeCalc();
        ~VolumeCalc();

        /**
         * @brief Calculates the volume of a given target
         *
         * @param frames Frames containing the images to be analyzed
         *
         * @param target Target that should be analyzed
         *
         * @return volume of the specified Target
         */
        double get_volume(const Frame * [] frames, const Target * target);
};

#endif // VOLUME_CALC_H_INCLUDED
