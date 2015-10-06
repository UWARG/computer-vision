/* 
    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.
 
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code must be explicitly referenced to WARG and this code
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

#include "json/json.h"
#include <iostream>
#include <ifstream>
using namespace std;

/*
* JSON Structure
* -One file per target type
* {
*      "parameters": {
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

class targetLoader{
      targetLoader(string f){
             file = f;     
      }
      public:
             TargetParameters getParameters();
             class TargetParameters{
                   public:
                          string* parametersTypes;
                          Parameter* parameters;
                          unsigned int parameterCount;
                          class Parameter{
                                public:
                                       bool enabled;
                                       double value;
                                       unsigned int confidence;
                          }
             }
      private:
              string file;
              readFile(string fileLocation);
              
}




TargetParameters targetLoader:getParameters(){
       return 0;
}

char* targetLoader:readFile(string fileLocation){
     ifstream jFile;
     char* memblock;
     
     jFile.open(fileLocation, ios::binary | ios::ate);
     if (file.is_open())
     {
        size = file.tellg();
        memblock = new char[size];
        file.seek(0, ios::beg);
        file.read(memblock,size);
        file.close();
     }
     return memblock;                   
}

Json::Value* targetLoader:readJSON(string jsonMessage){
     Json::Value* parsedFromString = new Json::Value();
     Json::Reader reader;
     
     reader.parse(jsonMessage, parsedFromString);
     if (parsingSuccessful){
        return parsedFromString;
     }
}
