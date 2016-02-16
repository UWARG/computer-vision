#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h"
#include <vector>

/**
* @brief this is for reading telemetry logs from the plane and store them into a linked list. prints a fatal log "error: failed to open the cdv file" on failure.
*
*
* @param filename the relative path and name of the file
* @return return the vector on success and return an empty vector on failure
*/

std::vector<Metadata> readcsv(const char* filename);

#endif // CSV_READING_H_INCLUDED
