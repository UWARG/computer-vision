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

class metadata_input{
    public:
        metadata_input();

        metadata_input(std::string filename);

        ~metadata_input();

        void input_file(std::string filename);

        void read_metadata();

        Metadata get_metadata(double timestamp);

        Metadata next_metadata();
    private:
        std::string filename;
        int mark=1;
        int photonum=0;
        int output_photonum=0;
        std::string heads[16];
        std::vector<Metadata> log;
};

#endif // CSV_READING_H_INCLUDED
