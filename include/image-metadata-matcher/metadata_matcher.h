#ifndef METADATA_MATCHER_H_INCLUDED
#define METADATA_MATCHER_H_INCLUDED

/** 
 * @file metadata_matcher.h
 *
 * @brief Module for matching frames with telemetry information that 
 *        corresponds to them
 *
 */
/*
    Notes:          Ideally this should use both the timestamp in the image's
                    exif metadata and the time that the plane records sending
                    the signal to take a picture plus whatever delay that
                    exists between when the signal is sent to the camera and
                    when the camera takes the photograph.
  */

class MetadataMatcher {
    public:
        /**
         * @brief Creates a MetadataMatcher for the telemetry file at the given path
         *
         * @param path Path of the telemetry file
         */
        MetadataMatcher(string path);

        ~MetadataMatcher();

        /**
         * @brief Fills in the given frame's metadata structure
         *
         * @param f Frame to be analysed
         */
        void matchFrame(Frame * f);
};

#endif // METADATA_MATCHER_H_INCLUDED
