#include "csvreading.h"
#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <cmath>
using namespace std;
using namespace boost;

MetadataInput::MetadataInput(string filename){
    string headrow;
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return;
    }
    getline(finput,headrow);
    int len=headrow.length();
    heads[0]="";
    int j=0;
    for(int i=0;i<len;i++){
        if(headrow[i]==','){
            j++;
       	    if(j==16){
                break;
            }
            heads[j]="";
        }
        else{
            heads[j]+=headrow[i];
        }
    }
    int temnum;
    bool f=0;
    char comma;
    while(!finput.eof()){
        Metadata input;
        for(int j=0;j<16;){
            if(heads[j].compare("time")==0){
                finput>>input.time;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("timeError")==0){
                finput>>input.timeError;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("lat")==0){
                finput>>input.lat;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("lon")==0){
                finput>>input.lon;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("latError")==0){
                finput>>input.latError;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("lonError")==0){
                finput>>input.lonError;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("pitch")==0){
                finput>>input.pitch;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("roll")==0){
                finput>>input.roll;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("pitchRate")==0){
                finput>>input.pitchRate;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("rollRate")==0){
                finput>>input.rollRate;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("yawRate")==0){
                finput>>input.yawRate;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("altitude")==0){
                finput>>input.altitude;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("heading")==0){
                finput>>input.heading;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("altError")==0){
                finput>>input.altError;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("headingError")==0){
                finput>>input.headingError;
                j++;
                if(j!=16){
                    finput>>comma;
                }
                continue;
            }
            if(heads[j].compare("cameraStatus")==0){
                finput>>temnum;
                if(temnum>cameraStatus){
                    f=1;
                }
                j++;
                continue;
            }
            cerr<<"header error"<<endl;
            return;
        }
        log.push_back(input);
        if(f==1){
            cameraStatus++;
            f=0;
        }
    }
    finput.close();
}

MetadataInput::~MetadataInput(){
}

void MetadataInput::push_back(Metadata newEntry){
    log.push_back(newEntry);
}

Metadata MetadataInput::bisearcher(double timestamp,int begin,int end){
    if(end-begin<=1){
        if(abs(timestamp-log.at(begin).time)<abs(timestamp-log.at(end).time)){
            return log.at(begin);
        }
        else{
            return log.at(end);
        }
    }
    if(timestamp<log.at((begin+end)/2).time){
        return bisearcher(timestamp,begin,(begin+end)/2);
    }
    else{
        return bisearcher(timestamp,(begin+end)/2,end);
    }
}

Metadata MetadataInput::get_metadata(double timestamp){
    return bisearcher(timestamp,0,log.size()-1);
}

Metadata MetadataInput::next_metadata(){
    output_cameraStatus++;
    if(output_cameraStatus>cameraStatus){
        BOOST_LOG_TRIVIAL(fatal)<<"error: no more metadata for now"<<endl;
        Metadata errorresult;
        return errorresult;
    }
    return log.at(output_cameraStatus-1);
}
