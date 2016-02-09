#include <fstream>
#include <string>
#include "metadata.h"
#include <vector>
#include <iostream>
using namespace std;

struct Metadatalinkedlist{
    Metadata a;
    Metadatalinkedlist* next;
};

Metadatalinkedlist* readcsv(const char* filename){
    Metadatalinkedlist* result=new Metadatalinkedlist;
    Metadatalinkedlist* poi=new Metadatalinkedlist;
    poi=result;
    poi->next=0;
    string headrow;
    ifstream finput(filename);
    getline(finput,headrow);
    int i=0,j=0;
    int len=headrow.length();
    string heads[14];
    heads[0]="";
    for(i=0;i<len;i++){
        if(headrow[i]==','){
	    j++;
       	    if(j==14){
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
	Metadatalinkedlist* temp=new Metadatalinkedlist;
	for(j=0;j<14;){
	    if(heads[j].compare("time")==0){
		finput>>temp->a.time;
		j++;
		if(j!=14){
		    finput>>comma;
		}
		continue;
	    }
	    if(heads[j].compare("timeError")==0){
	        finput>>temp->a.timeError;
	        j++;
	        if(j!=14){
	    	finput>>comma;
	        }
	        continue;
	    }
    	    if(heads[j].compare("lat")==0){
	        finput>>temp->a.lat;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("lon")==0){
	        finput>>temp->a.lon;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("latError")==0){
	        finput>>temp->a.latError;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("lonError")==0){
	        finput>>temp->a.lonError;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("pitch")==0){
	        finput>>temp->a.pitch;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("roll")==0){
	        finput>>temp->a.roll;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("pitchRate")==0){
	        finput>>temp->a.pitchRate;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("rollRate")==0){
	        finput>>temp->a.rollRate;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("yawRate")==0){
	        finput>>temp->a.yawRate;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("altitude")==0){
	        finput>>temp->a.altitude;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("heading")==0){
	        finput>>temp->a.heading;
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    if(heads[j].compare("photonum")==0){
	        finput>>temnum;
	        if(temnum>i){
		    f=1;
	        }
	        j++;
	        if(j!=14){
		    finput>>comma;
	        }
	        continue;
	    }
	    cerr<<"header error"<<endl;
	    return NULL;
	}
        if(f==1)
	{
	    i++;
    	    temp->next=0;
	    poi->next=temp;
            poi=temp;
	    f=0;
	}
    }
    return result->next;
}
