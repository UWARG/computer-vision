#ifndef VOLUME_CALC_H_INCLUDED
#define VOLUME_CALC_H_INCLUDED

/**
 * @file volume_calc.h
 * 
 * @brief Module for calculating the volume of a target using 
 *        a point cloud mesh
 *
 */

class volumeCalc {
    public:
        volumeCalc();
        ~volumeCalc();

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
