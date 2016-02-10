#ifndef CSV_READING_H_INCLUDED
#define CSV_READING_H_INCLUDED

#include "metadata.h";
using namespace std;

struct Metadatalinkedlist{
    Metadata a;
    Metadatalinkedlist* next;
};

/**
* @brief read a csv file creates a linked list of metadata
*
*
* @param filename the relative path and name of the file
* @return pointer to the first node of the linked list
*/

Metadatalinkedlist* readcsv(const char* filename);

#endif // CSV_READING_H_INCLUDED
