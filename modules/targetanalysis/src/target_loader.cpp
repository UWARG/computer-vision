/* 
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.
 
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include "target_loader.h"
using namespace std;
using namespace boost;

namespace logging = boost::log;
/*
* JSON Structure
* -One file per target type
* {
     "parameters": { 
        <parameter/category>: {
           "mean":{
              "enabled":<True/False>    ->Use this only as an override, the target_analyzer will have jurisdiction over 
              "value":<value>           ->Treated as a double. Don't put strings here (also remember to right the error checking code for this).
              "confidence":<value>      -> Integer value ranging from 0 to 100
           }
           "mode":{
              "enabled":<True/False>    ->Use this only as an override, the target_analyzer will have jurisdiction over 
              "value":<value>
              "confidence":<value>      -> Integer value ranging from 0 to 100
           }
           "std":{
              "enabled":<True/False>
              "value":<value>
              "confidence":<value>      -> Integer value ranging from 0 to 100
           }
           "distribution":{                  ->Examples like gaussian/binomial/etc
              "enabled":<True/False>
              "value":<value>           
              "confidence":<value>      -> Integer value ranging from 0 to 100
           }
        }
     } 
* }
*/


/*
TargetParameters targetLoader:getParameters(){
       return 0;
}
*/
TargetLoader::TargetLoader(const char* file){
   if (readFile(file)){
      std::string data = std::string(rawData);
      readJSON(data,&jsonParameters);
   }
}

bool TargetLoader::readFile(const char* fileLocation){
   ifstream jFile(fileLocation, ifstream::in | ios::ate);
  
   if (jFile.is_open()) {
      int size;
      size = jFile.tellg();
      rawData = new char[size];
      jFile.seekg(0, ios::beg);
      jFile.read(rawData,size);     
      jFile.close();
      BOOST_LOG_TRIVIAL(info) << "File opened: " << fileLocation;
      BOOST_LOG_TRIVIAL(info) << "Size: " << size;
   }
   else{
      BOOST_LOG_TRIVIAL(warning) << "Could not open file: " << fileLocation << "\n";
      return 0;
   }
   return 1;                   
}

bool TargetLoader::readJSON(string jsonMessage, property_tree::ptree* result){
   try{
      using property_tree::read_json;
      stringstream ss(jsonMessage);
      read_json(ss, *result);
   }
   catch(std::exception const& e){
      // Report to the user the failure and their locations in the document.
      BOOST_LOG_TRIVIAL(warning) << "Failed to parse configuration\n";
      return -1;
   }
   return 0;
}

property_tree::ptree* TargetLoader::getJSON(void){
   return &jsonParameters;
}

void TargetLoader::print()
{
   using boost::property_tree::ptree;
   ptree::const_iterator end = jsonParameters.end();
   for (ptree::const_iterator it = jsonParameters.begin(); it != end; ++it) {
      std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
      recursivePrint(it->second);
   }
}

void TargetLoader::recursivePrint(property_tree::ptree const& pt){
   using boost::property_tree::ptree;
   ptree::const_iterator end = pt.end();
   for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
      std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
      recursivePrint(it->second);
   }
}
