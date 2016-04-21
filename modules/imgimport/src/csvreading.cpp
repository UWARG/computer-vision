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
    int k=0;
    for(int i=0;i<len;i++){
        if(headrow[i]==','){
            k++;
       	    if(k==80){
                break;
            }
            heads[k]="";
        }
        else{
            if(headrow[i]!=13){
                heads[k]+=headrow[i];
            }
        }
    }
    int temnum;
    bool f=0;
    char comma;
    vector<vector<string>> readin;
    getline(finput,headrow);
    while(!finput.eof()){
        getline(finput,headrow);
        if(finput.eof())
            break;
        len=headrow.length();
        vector<string> buffer;
        buffer.resize(80);
        k=0;
        for(int i=0;i<len;i++){
            if(headrow[i]==','){
                k++;
       	        if(k==80){
                    break;
                }
            }
            else{
                if(headrow[i]!=13){
                    buffer.at(k)+=headrow[i];
                }
            }
        }
        readin.push_back(buffer);
    }
    log.resize(readin.size());
    for(int j=0;j<80;j++){
        if(heads[j].compare("sys_time")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).sys_time=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("time")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).time=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("lon")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).lon=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("lat")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).lat=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_rate")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_rate=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_rate_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_rate_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("pitch_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).pitch_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_rate")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_rate=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_rate_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_rate_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("roll_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).roll_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw_rate")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw_rate=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw_rate_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw_rate_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("yaw_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).yaw_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("airspeed")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).airspeed=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("heading")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).heading=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("heading_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).heading_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("heading_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).heading_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("heading_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).heading_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("heading_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).heading_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("altitude")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).altitude=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("altitude_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).altitude_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("altitude_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).altitude_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("altitude_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).altitude_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("altitude_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).altitude_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("flap_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).flap_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("flap_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).flap_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("flap_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).flap_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("flap_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).flap_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("ground_speed")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ground_speed=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("throttle_setpoint")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).throttle_setpoint=stof(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("throttle_kd")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).throttle_kd=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("throttle_kp")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).throttle_kp=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("throttle_ki")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).throttle_ki=stod(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("wireless_connection")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).wireless_connection=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("last_command_sent0")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).last_command_sent0=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("last_command_sent1")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).last_command_sent1=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("last_command_sent2")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).last_command_sent2=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("last_command_sent3")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).last_command_sent3=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("camera_status")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).camera_status=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("waypointindex")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).waypointindex=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("path_gain")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).path_gain=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("orbit_gain")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).orbit_gain=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("battery_level1")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).battery_level1=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("battery_level2")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).battery_level2=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch1in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch1in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch2in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch2in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch3in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch3in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch4in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch4in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch5in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch4in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch6in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch6in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch7in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch7in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch8in")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch8in=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch1out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch1out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch2out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch2out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch3out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch3out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch4out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch4out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch5out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch5out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch6out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch6out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch7out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch7out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("ch8out")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).ch8out=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("autopilot_active")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).autopilot_active=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("gps_status")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).gps_status=readin.at(c).at(j)[0];
            }
            continue;
        }
        if(heads[j].compare("path_checksum")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).path_checksum=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("waypoint_count")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).waypoint_count=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("waypoint_index")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).waypoint_index=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("path_following")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).path_following=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("autonomousLevel")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).autonomousLevel=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("startup_error_codes")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).startup_error_codes=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("startupSettings")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).startupSettings=stoi(readin.at(c).at(j));
            }
            continue;
        }
        if(heads[j].compare("RSSI")==0){
            for(int c=0;c<log.size();c++){
                log.at(c).RSSI=stoi(readin.at(c).at(j));
            }
            continue;
        }
        cerr<<"header error"<<endl;
        return;
    }
    finput.close();
    for(int j=0;j<log.size();j++){
        Metadata reader;
        reader.time=log.at(j).sys_time;
        reader.lat=log.at(j).lat;
        reader.lon=log.at(j).lon;
        reader.pitch=log.at(j).pitch;
        reader.roll=log.at(j).roll;
        reader.altitude=log.at(j).altitude;
        reader.heading=log.at(j).heading;
        bool end=(j==log.size()-1);
        if(j==0){
            reader.timeError=0;
            reader.altError=0;
            reader.headingError=0;
        }
        else{
            reader.timeError=(log.at(j).ground_speed-log.at(j-1).ground_speed)*(log.at(j).sys_time-log.at(j-1).sys_time);
            reader.altError=(log.at(j).pitch-log.at(j-1).pitch)*(log.at(j).sys_time-log.at(j-1).sys_time);
            reader.headingError=log.at(j).heading-log.at(j-1).heading;
        }
        if(j==0||end==1){
            reader.pitchRate=0;
            reader.rollRate=0;
            reader.yawRate=0;
        }
        else{
            reader.pitchRate=(double)(log.at(j+1).pitch-log.at(j-1).pitch)/(double)(log.at(j+1).sys_time-log.at(j-1).sys_time);
            reader.rollRate=(double)(log.at(j+1).roll-log.at(j-1).roll)/(double)(log.at(j+1).sys_time-log.at(j-1).sys_time);
            reader.yawRate=(double)(log.at(j+1).heading-log.at(j-1).heading)/(double)(log.at(j+1).sys_time-log.at(j-1).sys_time);
        }
        reader.latError=reader.timeError*(cos(reader.heading+reader.headingError)-cos(reader.heading-reader.headingError));
        reader.latError=reader.timeError*(sin(reader.heading+reader.headingError)-sin(reader.heading-reader.headingError));
        md.push_back(reader);
    }
}

MetadataInput::~MetadataInput(){
}

void MetadataInput::push_back(DataRead newEntry){
    log.push_back(newEntry);
    Metadata reader;
    int j=log.size()-1;
    reader.time=log.at(j).sys_time;
    reader.lat=log.at(j).lat;
    reader.lon=log.at(j).lon;
    reader.pitch=log.at(j).pitch;
    reader.roll=log.at(j).roll;
    reader.altitude=log.at(j).altitude;
    reader.heading=log.at(j).heading;
    if(j==0){
        reader.timeError=0;
        reader.altError=0;
        reader.headingError=0;
    }
    else{
        reader.timeError=(log.at(j).ground_speed-log.at(j-1).ground_speed)*(log.at(j).sys_time-log.at(j-1).sys_time);
        reader.altError=(log.at(j).pitch-log.at(j-1).pitch)*(log.at(j).sys_time-log.at(j-1).sys_time);
        reader.headingError=log.at(j).heading-log.at(j-1).heading;
    }
    reader.pitchRate=0;
    reader.rollRate=0;
    reader.yawRate=0;
    md.at(j-1).pitchRate=(double)(log.at(j).pitch-log.at(j-2).pitch)/(double)(log.at(j).sys_time-log.at(j-2).sys_time);
    md.at(j-1).rollRate=(double)(log.at(j).roll-log.at(j-2).roll)/(double)(log.at(j).sys_time-log.at(j-2).sys_time);
    md.at(j-1).yawRate=(double)(log.at(j).heading-log.at(j-2).heading)/(double)(log.at(j).sys_time-log.at(j-2).sys_time);
    reader.latError=reader.timeError*(cos(reader.heading+reader.headingError)-cos(reader.heading-reader.headingError));
    reader.latError=reader.timeError*(sin(reader.heading+reader.headingError)-sin(reader.heading-reader.headingError));
    md.push_back(reader);
}

Metadata MetadataInput::bisearcher(double timestamp,int begin,int end){
    if(end-begin<=1){
        if(abs(timestamp-log.at(begin).time)<abs(timestamp-log.at(end).time)){
            return md.at(begin);
        }
        else{
            return md.at(end);
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
    return md.at(output_cameraStatus-1);
}
