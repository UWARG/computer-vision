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
	
#ifndef GMAPS_MATCHER_H_INCLUDED
#define GMAPS_MATCHER_H_INCLUDED

/**
 * @file gmaps_matcher.h
 *
 * @brief Module for matching images to satellite imagery to be 
 *        used as a fallback in case telemetry data is not available
 *
 */

class GMapsMatcher {
    public:
        GMapsMatcher();
        ~GMapsMatcher();

        /**
         * @brief Puts the given frame into the analysis queue
         * Frame will be freed after analysis
         *
         * @param f Frame containing the targets to analyze
         */
        void analyze_targets_in_frame(Frame * f);

        /**
         * @brief retrieves a vector containing all of the unique Targets that 
         *        have been indentified. 
         *        The caller of the function should not delete the Targets as they
         *        will be deleted by the TargetAnalyzer destructor
         *        
         * @return vector containing the unique Targets which have been analyzed
         */
        vector<Target *> get_processed();
};

#endif // GMAPS_MATCHER_H_INCLUDED
