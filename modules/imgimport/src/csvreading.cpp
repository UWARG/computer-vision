#include "csvreading.h"
#include <fstream>
#include <string>
#include <iostream>
#include <boost/log/trivial.hpp>
using namespace std;
using namespace boost;

vector<Metadata> readcsv(const char* filename){
    vector<Metadata> result;
    vector<Metadata> errorResult;
    string headrow;
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return errorResult;
    }
    getline(finput,headrow);
    int i=0,j=0;
    int len=headrow.length();
    string heads[16];
    heads[0]="";
    for(i=0;i<len;i++){
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
    i=0;
    int temnum;
    bool f=0;
    char comma;
    while(!finput.eof()){
        Metadata input;
        for(j=0;j<16;){
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
                if(temnum>=i){
                    f=1;
                }
                j++;
                continue;
            }
            cerr<<"header error"<<endl;
            return errorResult;
        }
        if(f==1){
            result.push_back(input);
            i++;
            f=0;
        }
    }
    finput.close();
    return result;
}
