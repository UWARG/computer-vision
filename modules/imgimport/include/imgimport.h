/**
 * @file imgimport.h
 * @author WARG
 *
 * @section LICENSE
 *
 * Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 * All rights reserved.
 *
 * This software is licensed under a modified version of the BSD 3 clause license
 * that should have been included with this software in a file called COPYING.txt
 * Otherwise it is available at:
 * https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef IMAGE_IMPORT_H_INCLUDED
#define IMAGE_IMPORT_H_INCLUDED

#include "metadata_input.h"
#include "frame.h"

/**
 * @class ImageImport
 *
 * @brief Abstract class for importing frames and corresponding metadata
 *
 */

class ImageImport {
public:
    /**
     * @brief
     * *
     * @param reader
     */
    ImageImport(MetadataInput * reader);

    virtual ~ImageImport();

    /**
     * @brief Retrieves the next frame to be analyzed
     *
     * @return Frame to be analyzed
     */
    virtual Frame * next_frame() = 0;

protected:
    MetadataInput * reader;
};

#endif // IMAGE_IMPORT_H_INCLUDED
