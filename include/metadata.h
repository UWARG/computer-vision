#ifndef METADATA_H_INCLUDED
#define METADATA_H_INCLUDED

// ----------------------------------------------------------------------------
/**
    File: metadata.h
  
    System:         Post Processing
    Component Name: Metadata
    Language: C++
  
    (c) Copyright UWARG 2015
  
    Description: Structure for storing plane status information from the time
                that an image was taken
  
**/
// ----------------------------------------------------------------------------

struct Metadata{
    double time
    double lat, lon;
    double pitch, roll, yaw;
    double pitchRate, rollRate, yawRate;
    double altitude, heading;
};


#endif // METADATA_H_INCLUDED
