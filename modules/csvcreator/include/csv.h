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
#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

/**
 * @file csv.h
 *
 * @brief Module for creating formatting csv files to store target data
 *
**/

#include "target.h"

/**
 * @brief Creates the file "Results.csv" using the information from the Targets given
 *
 * @param targets Targets that should be included in the CSV
 */
void create_csv(Target * [] targets);

#endif // CSV_H_INCLUDED
