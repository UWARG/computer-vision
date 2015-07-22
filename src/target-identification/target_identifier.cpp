#include "target-identification/target_identifier.h"

TargetIdentifier::TargetIdentifier(){
    //detector = new ObjectDetector();
}

TargetIdentifier::~TargetIdentifier(){
    delete detector;
}

void TargetIdentifier::process_frame(Frame * f){
    detector->process_frame(f);
}
