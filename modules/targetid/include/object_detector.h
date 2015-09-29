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
	
#ifndef OBJECT_DETECTOR_H_INCLUDED
#define OBJECT_DETECTOR_H_INCLUDED

#include "frame.h"
#include "filter.h"
#include "contour_creator.h"

class ObjectDetector {
    Filter * filter;
    ContourCreator * ccreator;
    public:
        ObjectDetector(Filter * filter, ContourCreator * ccreator);
        void process_frame(Frame * f);
        ~ObjectDetector();
};

#endif // OBJECT_DETECTOR_H_INCLUDED
