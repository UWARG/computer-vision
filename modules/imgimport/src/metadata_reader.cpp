/**
 * @file metadata_reader.cpp
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

#include "metadata_reader.h"
#include <boost/algorithm/string.hpp>
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

MetadataReader::MetadataReader(MetadataInput &input) : input(input) {

}

MetadataReader::MetadataReader(MetadataInput &input, string filename) : MetadataReader(input) {
    string line;
    ifstream finput(filename);
    if(!finput.is_open()) {
        throw new runtime_error("failed to open the csv file");
    }
    getline(finput, line);
    set_head_row(line);

    bool f = false;
    char comma;
    while(!finput.eof()){
        getline(finput, line);
        if(finput.eof())
            break;

        vector<string> args;
        boost::split(args, line, boost::is_any_of(" "));
        unordered_map<string, string> kvp;
        if (args.size() != heads.size()) continue;
        for (int i = 0; i < args.size(); i++) {
            kvp.insert({heads[i], args[i]});
        }
        input.push_back(kvp);
    }

    finput.close();
}

void MetadataReader::read_log() {
    enum { max_length = 1024 };

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), addr.c_str(), port.c_str());
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    while(true) {
        char reply[max_length];
        size_t reply_length = boost::asio::read(s,
            boost::asio::buffer(reply, max_length));
        for (int i = 0; i < reply_length; i++) {
            buffer += reply[i];
        }
        int eol;
        if ((eol = buffer.find_first_of('\n')) != string::npos) {
            string line = buffer.substr(0, eol);
            buffer = buffer.substr(eol + 1, string::npos);
            BOOST_LOG_TRIVIAL(debug) << "reading line " << line;
            if (heads.size() == 0) {
                set_head_row(line);
            } else {
                vector<string> args;
                boost::split(args, line, boost::is_any_of(" "));
                unordered_map<string, string> kvp;
                if (args.size() != heads.size()) continue;
                for (int i = 0; i < args.size(); i++) {
                    kvp.insert({heads[i], args[i]});
                }
                input.push_back(kvp);
            }
        }
    }
}

MetadataReader::MetadataReader(MetadataInput &input, string addr, string port) : input(input), addr(addr), port(port), buffer("") {
    ioService.post(boost::bind(&MetadataReader::read_log, this));
    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService));
}

void MetadataReader::set_head_row(string headRow) {
    int len=headRow.length();
    heads.push_back("");
    int k=0;
    for(int i=0;i<len;i++){
        if(headRow[i]==','){
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

MetadataReader::~MetadataReader(){
    ioService.stop();
}
