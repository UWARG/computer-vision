#ifndef IMAGE_IMPORT_H_INCLUDED
#define IMAGE_IMPORT_H_INCLUDED

/** 
 * @file imgimport.h
 *
 * @brief Module for matching frames with telemetry information that 
 *        corresponds to them
 *
 * @section Notes   
 *
 * Ideally this should use both the timestamp in the image's
 * exif metadata and the time that the plane records sending
 * the signal to take a picture plus whatever delay that
 * exists between when the signal is sent to the camera and
 * when the camera takes the photograph.
 *
 */

#include "frame.h"
#include <string>

class ImageImport {
    public:
        /**
         * @brief Creates a ImageImport for the telemetry file at the given path
         *
         * @param path Path of the telemetry file
         */
        ImageImport(std::string path);

        ~ImageImport();

        /**
         * @brief Fills in the given frame's metadata structure
         *
         * @param f Frame to be analysed
         */
        Frame * next_frame();
};

#endif // IMAGE_IMPORT_H_INCLUDED
