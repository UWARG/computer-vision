#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h";

struct Metadatalinkedlist{
    Metadata a;
    Metadatalinkedlist* next;
};

/**
* @brief this is for reading telemetry logs from the plane and store them into a linked list 
*
*
* @param filename the relative path and name of the file
* @error if the file opening fails, print a fatal log "error: failed to open the cdv file" and return NULL
* @return pointer to the first node of the linked list
*/

Metadatalinkedlist* readcsv(const char* filename);

#endif // CSV_READING_H_INCLUDED
