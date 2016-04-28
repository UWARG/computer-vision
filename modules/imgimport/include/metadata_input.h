/**
 * @file metadata_input.h
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

#ifndef METADATA_INPUT_H_INCLUDED
#define METADATA_INPUT_H_INCLUDED

#include "metadata.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>

/**
 * @Class MetadataInput
 * @brief A class for reading telemetry logs and searching for log entries
 */
class MetadataInput{
public:
    MetadataInput();

    /*
     * @brief constructor for MetadataInput which reads log from a file
     *
     * Throws a runtime error if the file cannot be read
     *
     * @param filename Path of the file to be read
     */
    MetadataInput(std::string filename);

    /**
     * @brief Constructor for MetadataInput for reading log over the network
     *
     * @param addr IP address to connect to which the log will be sent from
     * @param port port to connect to
     */
    MetadataInput(std::string addr, std::string port);

    ~MetadataInput();

    /**
     * @brief searches for the closest log entry to the given timestamp
     *
     * Probably throws an exception if no entries are in the log
     *
     * @param timestamp the time in the format hhmmss.ms
     * @returns the closest log entry to the given timestamp
     */
    Metadata get_metadata(double timestamp);

    /**
     * @brief retrieves the next log entry corresponding to a time when the log notes a photograph was triggered
     *
     * Specifically returns the next log entry where the value of cameraStatus is one less than the previous entry
     * If called more times then there are such entries, will continuously return the last such entry
     *
     * @return The next log entry when the log notes a photograph was triggered
     */
    Metadata next_metadata();

private:

    /**
     * @brief Creates a Metadata structure for the log entry at the given index
     *
     * The passed timestamp will be used for calculating time-based error and is the
     * time that the Metadata struct will be used for (i.e. time of the photo), not the
     * time in the log entry.
     *
     * @param index Index of the log entry to use
     * @param timestamp the timestamp used in the original query
     * @return A metadata structure for the log entry at the given index
     */
    Metadata metadata_at_index(int index, double timestamp);

    /*
     * @brief helper for running binary search on the log
     *
     * @param value value to be queried
     * @param begin min position in the log to search
     * @param end max position in the log to search
     * @param field column of the log to query
     * @return result of the query
     */
    Metadata bisearcher(double value, int begin, int end, std::string field);

    /**
     * @brief Adds a new entry to the log
     *
     * @param newEntry csv row representing a telemetry log entry
     */
    void push_back(std::string newEntry);

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

    int cameraStatus=0;

    int size;

    /**
     * Index of the time header in the log
     */
    int timeIndex;

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
};

#endif // METADATA_INPUT_H_INCLUDED
