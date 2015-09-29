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
