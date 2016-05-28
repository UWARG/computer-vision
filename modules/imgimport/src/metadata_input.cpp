/**
 * @file metadata_input.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 * Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 * All rights reserved.
 *
 * This software is licensed under a modified version of the BSD 3 clause license
 * that should have been included with this software in a file called COPYING.txt
 * Otherwise it is available at:
 * https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#include "metadata_input.h"
#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <cmath>
using namespace std;
using namespace boost;

MetadataInput::MetadataInput() : size(0) {

}

MetadataInput::MetadataInput(string filename) : MetadataInput() {
    string line;
    ifstream finput(filename);
    if(!finput.is_open())
    {
        BOOST_LOG_TRIVIAL(fatal)<<"error: failed to open the csv file"<<endl;
        return;
    }
    getline(finput, line);
    set_head_row(line);

    bool f = false;
    char comma;
    while(!finput.eof()){
        getline(finput, line);
        if(finput.eof())
            break;

        push_back(line);
    }

    finput.close();
}

void MetadataInput::set_head_row(string headRow) {
    int len=headRow.length();
    heads.push_back("");
    int k=0;
    for(int i=0;i<len;i++){
        if(headRow[i]==','){
            if (heads[k].compare("time") == 0) {
                timeIndex = k;
            }
            k++;
            heads.push_back("");
        }
        else{
            if(headRow[i]!=13){
                heads[k]+=headRow[i];
            }
        }
    }
}

void MetadataInput::push_back(string newEntry) {
    // TODO: Use a proper string split function
    int len = newEntry.length();
    len = newEntry.length();
    vector<string> buffer(heads.size());
    int k = 0;
    for(int i = 0; i < len; i++){
        if(newEntry[i]==','){
            k++;
            if(k == heads.size()){
                break;
            }
        }
        else{
            if(newEntry[i] != 13){
                buffer.at(k) += newEntry[i];
            }
        }
    }
    if (buffer.size() < heads.size() || abs(stod(buffer[timeIndex])) < 1) return; // ignore missing entries
    for (int i = 0; i < heads.size(); i++) {
        data[heads[i]].push_back(buffer[i]);
    }
    size++;
}

MetadataInput::~MetadataInput(){
}

Metadata MetadataInput::bisearcher(double value,int begin,int end, string field){
    if (end - begin <= 1) {
        if (abs(value - stod(data[field].at(begin))) < abs(value - stod(data[field].at(end)))) {
            return metadata_at_index(begin, field.compare("time") == 0 ? value : stod(data["time"].at(begin)));
        }
        else {
            return metadata_at_index(end, field.compare("time") == 0 ? value : stod(data["time"].at(end)));
        }
    }
    if (value < stod(data[field].at((begin + end)/2))) {
        return bisearcher(value, begin, (begin + end)/2, field);
    }
    else{
        return bisearcher(value, (begin + end)/2, end, field);
    }
}

Metadata MetadataInput::metadata_at_index(int index, double timestamp) {
    Metadata reader;
    reader.time = stod(data["time"].at(index));
    reader.lat = stod(data["lat"].at(index));
    reader.lon = stod(data["lon"].at(index));
    reader.pitch = stod(data["pitch"].at(index));
    reader.roll = stod(data["roll"].at(index));
    reader.altitude = stod(data["altitude"].at(index));
    reader.heading = stod(data["heading"].at(index));
    BOOST_LOG_TRIVIAL(debug) << "Fetching metadata at index " << index << " requested timestamp is " << timestamp << " time found is " << reader.time;

    // TODO: fill in error info
    return reader;
}

Metadata MetadataInput::get_metadata(double timestamp){
    return bisearcher(timestamp, 0, size-1, "time");
}

Metadata MetadataInput::next_metadata() {
    return bisearcher(cameraStatus++, 0, size - 1, "cameraStatus");
}
