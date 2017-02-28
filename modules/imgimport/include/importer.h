/**
 * @file importer.h
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

#ifndef IMPORTER_H_INCLUDED
#define IMPORTER_H_INCLUDED

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <queue>
#include "metadata_input.h"
#include "imgimport.h"

class Source;

/**
 * @class Importer
 *
 * @brief Class for reading image files from a directory
 */

class Importer {
public:
    /**
     * @brief Creates an Importer object
     */
    Importer();

    ~Importer();

    /**
     *  @brief adds an image source
     *  @param importer The image source
     *  @param msdelay
     */
    void add_source(ImageImport *importer, long msdelay);

    /**
     * @brief Retrieves the next frame to be analyzed
     *
     * @return Frame to be analyzed
     */
    Frame* next_frame();

    /**
     * Removes source
     * @param index Index of the source to be removed
     */
    void remove_source(int index);

    /**
     * Updates delay for the source
     * @param index Index of the source to update
     * @param delayms new delay in milliseconds
     */
    void update_delay(int index, long msdelay);
private:
    boost::asio::io_service ioService;
    std::vector<Source *> sources;
    std::priority_queue<Frame *> cache;
};

#endif // IMPORTER_H_INCLUDED
