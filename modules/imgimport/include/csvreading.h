#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h"
#include <vector>
#include <string>

/**
* @brief this is for reading telemetry logs from the plane and store them into a vector. prints a fatal log "error: failed to open the csv file" on failure.
*
*
* @param filename the relative path and name of the file
* @return return the vector on success and return an empty vector on failure
*/

class MetadataInput{
    public:
        MetadataInput(std::string filename);

        ~MetadataInput();

        void push_back(Metadata newEntry);

        Metadata bisearcher(double timestamp,int begin,int end);

        Metadata get_metadata(double timestamp);

        Metadata next_metadata();
    private:
        int cameraStatus=0;
        int output_cameraStatus=0;
        std::string heads[16];
        std::vector<Metadata> log;
};

#endif // CSV_READING_H_INCLUDED
