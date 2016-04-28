#ifndef METADATA_INPUT_H_INCLUDED
#define METADATA_INPUT_H_INCLUDED

#include "metadata.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <boost/asio.hpp>

/**
* @brief this is for reading telemetry logs from the plane and store them into a vector. prints a fatal log "error: failed to open the csv file" on failure.
*
*
* @param filename the relative path and name of the file
* @return return the vector on success and return an empty vector on failure
*/

class MetadataInput{
    public:
        MetadataInput();

        MetadataInput(std::string filename);

        MetadataInput(std::string addr, std::string port);

        ~MetadataInput();

        Metadata get_metadata(double timestamp);

        Metadata next_metadata();

    private:
        Metadata metadata_at_index(int index, double timestamp);

        Metadata bisearcher(double value, int begin, int end, std::string field);

        void push_back(std::string newEntry);

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

        std::vector<std::string> heads;
        std::unordered_map<std::string, std::vector<std::string> > data;

        boost::asio::io_service ioService;
        std::string addr, port;
        std::string buffer;
};

#endif // METADATA_INPUT_H_INCLUDED
