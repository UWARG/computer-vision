/**
 * @file importer.cpp
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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <thread>
#include "importer.h"
#include "frame.h"

using namespace std::this_thread;
using namespace std::chrono;
using namespace std;

class Source {
public:
    Source(ImageImport* importer, priority_queue<Frame*> &cache, long msdelay, const int &bufferSize)
            : importer(importer), cache(cache), msdelay(msdelay), running(true), bufferSize(bufferSize) {

    }

    ~Source() {
        delete importer;
    }

    /**
     *  @brief Fetches next frame
     *  If msdelay is set to zero (or less) it will wait until
     *  the cache is contains less than bufferSize frames
     *  before taking any more frames from this source
     */
    Frame * fetch_frames() {
        Frame *frame;
        while (running) {
            if (msdelay > 0) {
                if (this->cache.size() < bufferSize) {
                    if ((frame = importer->next_frame()) != NULL) {
                        this->cache.push(frame);
                    }
                    sleep_for(milliseconds(msdelay));
                } else {
                    sleep_for(milliseconds(10));
                }
            } else {
                if (this->cache.size() < bufferSize && (frame = importer->next_frame()) != NULL) {
                    this->cache.push(frame);
                }
                sleep_for(milliseconds(10));
            }
        }
    }

    void stop() {
        running = false;
    }

    void set_delay(long msdelay) {
        this->msdelay = msdelay;
    }
private:
    ImageImport *importer;
    priority_queue<Frame*> &cache;
    long msdelay;
    bool running;
    const int &bufferSize;
};

Importer::Importer(): bufferSize(0) {

}

void Importer::add_source(ImageImport *importer, long msdelay) {
    Source *source = new Source(importer, cache, msdelay, bufferSize);
    sources.push_back(source);
    ioService.post(boost::bind(&Source::fetch_frames, source));
    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService));
}

void Importer::remove_source(int index) {
    sources[index]->stop();
    sources.erase(sources.begin() + index);
}

void Importer::update_delay(int index, long msdelay) {
    sources[index]->set_delay(msdelay);
}

Importer::~Importer() {
    while (cache.size() > 0) {
        Frame *frame = cache.top();
        delete frame;
        cache.pop();
    }
    for (Source *source: sources) {
        delete source;
    }
}

Frame* Importer::next_frame() {
    if (cache.size() == 0) return NULL;
    Frame *top = cache.top();
    cache.pop();
    return top;
}

int Importer::num_sources() {
    return sources.size();
}

void Importer::set_buffer_size(int bufferSize) {
    this->bufferSize = bufferSize;
}

int Importer::get_buffer_size() {
    return this->bufferSize;
}
