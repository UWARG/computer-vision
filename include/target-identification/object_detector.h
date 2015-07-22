#ifndef OBJECT_DETECTOR_H_INCLUDED
#define OBJECT_DETECTOR_H_INCLUDED

#include "frame.h"
#include "target-identification/filter.h"
#include "target-identification/contour_creator.h"

class ObjectDetector {
    Filter * filter;
    ContourCreator * ccreator;
    public:
        ObjectDetector(Filter * filter, ContourCreator * ccreator);
        void process_frame(Frame * f);
        ~ObjectDetector();
};
