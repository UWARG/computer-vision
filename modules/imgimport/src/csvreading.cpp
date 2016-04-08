#include "csvreading.h"
#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
using namespace std;
using namespace boost;

metadata_input::metadata_input(){
}

metadata_input::metadata_input(string filename){
    this->filename=filename;
    string headrow;
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return;
    }
    getline(finput,headrow);
    finput.close();
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
}

metadata_input::~metadata_input(){
}

void metadata_input::input_file(string filename){
    this->filename=filename;
    string headrow;
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return;
    }
    getline(finput,headrow);
    finput.close();
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
}


void metadata_input::read_metadata(){
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return;
    }
    string row;
    for(int i=0;i<mark;i++){
        getline(finput,row);
    }
    int temnum;
    bool f=0;
    char comma;
    while(!finput.eof()){
        Metadata input;
        for(int j=0;j<16;){
            if(heads[j].compare("time")==0){
                finput>>input.time;
                if(finput.eof()){
                    break;
                }
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
            if(heads[j].compare("photonum")==0){
                finput>>temnum;
                if(temnum>photonum){
                    f=1;
                }
                j++;
                continue;
            }
            cerr<<"header error"<<endl;
            return;
        }
        mark++;
        if(f==1){
            log.push_back(input);
            photonum++;
            f=0;
        }
    }
    finput.close();
}

Metadata metadata_input::get_metadata(double timestamp){
    read_metadata();
    for(int i=0;i<log.size();i++){
        if(log.at(i).time==timestamp){
            return log.at(i);
        }
    }
    BOOST_LOG_TRIVIAL(fatal)<<"error: cannot find metadata at given timestamp"<<endl;
    Metadata errorresult;
    return errorresult;
}

Metadata metadata_input::next_metadata(){
    read_metadata();
    output_photonum++;
    if(output_photonum>photonum){
        BOOST_LOG_TRIVIAL(fatal)<<"error: no more metadata for now"<<endl;
        Metadata errorresult;
        return errorresult;
    }
    return log.at(output_photonum-1);
}
