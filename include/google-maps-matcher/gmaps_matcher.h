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
        void analyzeTargetsInFrame(Frame * f);

        /**
         * @brief retrieves a vector containing all of the unique Targets that 
         *        have been indentified. 
         *        The caller of the function should not delete the Targets as they
         *        will be deleted by the TargetAnalyzer destructor
         *        
         * @return vector containing the unique Targets which have been analyzed
         */
        vector<Target *> getProcessed();
};

#endif // GMAPS_MATCHER_H_INCLUDED
