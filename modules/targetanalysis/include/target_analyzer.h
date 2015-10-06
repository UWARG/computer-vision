/*This file is part of WARG's computer-vision.

Copyright (c) 2015, WARG
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

4. Any code used MUST be referenced to WARG (Waterloo Aerial Robotics Group). Additionally, this code cannot be used in any competition against WARG.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	*/
	
#ifndef TARGET_ANALYZER_H_INCLUDED
#define TARGET_ANALYZER_H_INCLUDED

/**
 * @file target_analyzer.h
 *
 * @brief Module for analyzing individual PixelTargets and combining them into 
 *        a set of unique Targets
 *
 * Module geolocates targets using their pixel locations
 * and photo metadata, determines target type and calculates 
 * possible error. As targets are processed unique targets will 
 * be identified and the data combined into a single object.
 *
 * The TargetAnalyzer does not delete any Frames or PixelTargets
 *
**/

#include "frame.h"

class TargetAnalyzer {
    static TargetAnalyzer * analyzer;
    TargetAnalyzer();

    void cleanup();                
    public:

        static const TargetAnalyzer * getInstance();

        /**
         * @brief Analyzes a pixeltarget and returns a pointer to the unique target 
         *        it belongs to
         *
         * @param p PixelTarget to be analyzed
         * @param f Frame that the PixelTarget belongs to
         * @return pointer to the Target that the PixelTarget belongs to
         */
        Target * analyze_targets_in_frame(PixelTarget * p, Frame * f);

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

#endif // TARGET_ANALYZER_H_INCLUDED
