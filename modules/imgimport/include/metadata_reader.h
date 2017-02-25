/**
 * @file metadata_reader.h
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

#ifndef METADATA_READER_H_INCLUDED
#define METADATA_READER_H_INCLUDED

#include "metadata_input.h"
#include "metadata.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>

/**
 * @class MetadataReader
 * @brief A class for reading telemetry logs
 */
class MetadataReader{
public:

    /*
     * @brief constructor for MetadataReader which reads log from a file
     *
     * Throws a runtime error if the file cannot be read
     *
     * @param filename Path of the file to be read
     */
    MetadataReader(MetadataInput &input, std::string filename);

    /**
     * @brief Constructor for MetadataReader for reading log over the network
     *
     * @param addr IP address to connect to which the log will be sent from
     * @param port port to connect to
     */
    MetadataReader(MetadataInput &input, std::string addr, std::string port);

    ~MetadataReader();
private:
    MetadataReader(MetadataInput &input);

    /**
     * @brief Sets the column headers for the log
     * @param csv row containing column headers
     */
    void set_head_row(std::string headRow);

    /**
     *  @brief Reads the telemetry log over the network
     *
     *  Connecs to the address and port stored in addr and port fields
     *  Reads reply and stores it in the log
     *  Blocks forever and should only be used inside another thread
     */
    void read_log();

    /**
     * Column Headers for the log
     */
    std::vector<std::string> heads;

    /**
     * Map mapping headers to their columns
     * e.g. data["time"] gives a vector containing each time cell in the log, in
     *  increasing order from the first entry received
     */
    std::unordered_map<std::string, std::vector<std::string> > data;

    boost::asio::io_service ioService;
    std::string addr, port;
    std::string buffer;

    MetadataInput &input;
};

#endif // METADATA_READER_H_INCLUDED
