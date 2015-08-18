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
