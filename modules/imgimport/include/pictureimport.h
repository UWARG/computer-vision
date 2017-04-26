/**
 * @file picture_import.h
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

#ifndef PICTURE_IMPORT_H_INCLUDED
#define PICTURE_IMPORT_H_INCLUDED

#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include "metadata_input.h"
#include <dirent.h>
#include "imgimport.h"


/**
 * @class PictureImport
 *
 * @brief Class for reading image files from a directory
 */

class PictureImport : public ImageImport {
public:
    /**
     * @brief Creates a PictureImport object
     * @param filePath directory containing image files
     */
    PictureImport(std::string filePath, MetadataInput * mdin, Camera &camera);

    ~PictureImport();

    /**
     * @brief Retrieves the next frame to be analyzed
     *
     * @return Frame to be analyzed
     */
    Frame* next_frame();

    /**
     * @brief String representation of the Importer
     */
    virtual std::string to_string();
private:
    std::string filePath;
    DIR* dr;
    MetadataInput* mdin;
};

#endif // PICTURE_IMPORT_H_INCLUDED
