#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

// ----------------------------------------------------------------------------
/**
    @file csv.h
     
    @brief Module for creating formatting csv files to store target data
  
**/
// ----------------------------------------------------------------------------

#include "target.h"

/**
 * @brief Creates the file "Results.csv" using the information from the Targets given
 *
 * @param targets Targets that should be included in the CSV
 */
void createCSV(Target * [] targets);

#endif // CSV_H_INCLUDED
