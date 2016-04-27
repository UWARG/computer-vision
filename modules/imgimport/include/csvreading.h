#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h"
#include <vector>
#include <string>
#include <unordered_map>

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

        ~MetadataInput();

        Metadata get_metadata(double timestamp);

        Metadata next_metadata();

    private:
        Metadata metadata_at_index(int index, double timestamp);

        Metadata bisearcher(double value, int begin, int end, std::string field);

        void push_back(std::string newEntry);

        void set_head_row(std::string headRow);

        int cameraStatus=0;

        int size;

        /**
         * Index of the time header in the log
         */
        int timeIndex;

        std::vector<std::string> heads;
        std::unordered_map<std::string, std::vector<std::string> > data;
};

#endif // CSV_READING_H_INCLUDED
