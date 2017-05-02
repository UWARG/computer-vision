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
#include "metadata_reader.h"
#include <fstream>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <cmath>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

MetadataInput::MetadataInput() : size(0) {

}

void MetadataInput::push_back(unordered_map<string, string> newEntry) {
    if (newEntry.find("gps_time") == newEntry.end()) {
        BOOST_LOG_TRIVIAL(error) << "Trying to add an entry with no time column";
        return;
    }

    if (newEntry["gps_time"].size() <= 1) {
        BOOST_LOG_TRIVIAL(trace) << "gps_time cell is not filled";
        return;
    }
    int index = size, upper = size, lower = 0;
    size++;
    long newTime = (long)stod(newEntry["gps_time"]);
    while((index < size - 1 && index >= 0 && time[index] < newTime)
                || (index > 0 && index < size && time[index - 1] > newTime)) {
        if (index == size - 1 && time[index - 1] > newTime) {
            upper = index;
            index = (index + lower)/2;
        } else if (time[index] > newTime) {
            upper = index;
            index = (index + lower)/2;
        } else {
            lower = index;
            index = (index + upper)/2;
        }
    }
    for (pair<string, string> p : newEntry) {
        if (index == size - 1) {
            data[p.first].push_back(p.second);
        } else {
            data[p.first].insert(data[p.first].begin() + index, p.second);
        }
    }
    if (index == size - 1) {
        time.push_back(newTime);
    } else {
        time.insert(time.begin() + index, newTime);
    }
}

MetadataInput::~MetadataInput(){
    for (int i = 0; i < sources.size(); i++) {
        delete sources[i];
    }
}

Metadata MetadataInput::bisearcher(double value,int begin,int end, string field){
    if (end - begin <= 1) {
        if (abs(value - stod(data[field].at(begin))) < abs(value - stod(data[field].at(end)))) {
            return metadata_at_index(begin, field.compare("gps_time") == 0 ? value : stod(data["gps_time"].at(begin)));
        }
        else {
            return metadata_at_index(end, field.compare("gps_time") == 0 ? value : stod(data["gps_time"].at(end)));
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
    reader.time = stod(data["gps_time"].at(index));
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
    if (size == 0) throw runtime_error("no log entries available");
    return bisearcher(timestamp, 0, size-1, "gps_time");
}

Metadata MetadataInput::next_metadata() {
    if (size == 0) throw runtime_error("no log entries available");
    return bisearcher(cameraStatus++, 0, size - 1, "cameraStatus");
}

void MetadataInput::add_source(MetadataReader *reader) {
    sources.push_back(reader);
}

int MetadataInput::num_sources() {
    return sources.size();
}

int MetadataInput::check_data_order() {
    vector<string> time = data["gps_time"];
    for (int i = 1; i < time.size(); i++) {
        if (stol(time[i]) < stol(time[i - 1])) {
            return i;
        }
    }
    return 0;
}
