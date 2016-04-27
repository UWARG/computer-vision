/* 
    This file is part of WARG's computer-vision

    Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
    All rights reserved.
 
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Usage of this code MUST be explicitly referenced to WARG and this code
       cannot be used in any competition against WARG.
    4. Neither the name of the WARG nor the names of its contributors may be used
       to endorse or promote products derived from this software without specific
       prior written permission.

    THIS SOFTWARE IS PROVIDED BY WARG ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL WARG BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <utility>
#include <vector>

#include <errno.h>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

//If not using Decklink, omit the rest of the code. Note, if more cameras are added into this suite, this will need to change.
#ifdef HAS_DECKLINK

#include <DeckLinkAPI.h>
#include "ComPtr.h"
#include "DeckLinkCapture.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/property_tree/ptree.hpp>

#include "decklink_import.h"

using namespace boost;

namespace logging = boost::log;

std::vector<DeckLinkCapture> captures;
IDeckLink* deckLink;

Frame* img;

DecklinkImport::DecklinkImport() : ImageImport(){
    initVideoSource();
    startCapture();
    img = (Frame*) malloc(sizeof(Frame));
}

DecklinkImport::~DecklinkImport(){
    stopCapture();
    free(img);
}

int DecklinkImport::initVideoSource()
{
    ComPtr<IDeckLinkIterator> deckLinkIterator = CreateDeckLinkIteratorInstance();
    if (! deckLinkIterator) {
        BOOST_LOG_TRIVIAL(error) << "This application requires a DeckLink driver. \n";
        return 1;
    }

    while (deckLinkIterator->Next(&deckLink) == S_OK) {
        captures.push_back(std::move(DeckLinkCapture(ComPtr<IDeckLink>(deckLink))));
      
    }

    if (captures.size() == 0) {
        BOOST_LOG_TRIVIAL(error) << program_invocation_short_name
                  		 << ": No DeckLink device was found"
                  		 << std::endl;
        return 2;
    }
    return 0; 

}

int DeckLinkImport::startCapture(){
    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        if (! capture.start()){
            BOOST_LOG_TRIVIAL(error) << program_invocation_short_name
                      		     << ": Could not start the capture on the device.'"
                      		     << capture.getDeviceDisplayName()
                      		     << "': "
                      		     << capture.errorString()
                      		     << std::endl;
        return 1;
        }
    }
    return 0;
}

int DecklinkImport::stopCapture(){
    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        capture.stop();
    }
    return 0;
}

int DecklinkImport::grabFrame(cv::Mat* frame){
    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        capture.grab();
    }
        for(unsigned i = 0; i < captures.size(); ++i) {
            captures[i].retrieve(*frame);
        }
    return 0;
}

Frame* DecklinkImport::next_frame(){
    cv::Mat oFrame;
    grabFrame(&oFrame);
    //Insert string id and metadata once a ID generator has been coded and the metadata generator has been coded.
    Metadata m;
    Frame* img = new Frame(&oFrame, "ab", Metadata());
    return img;
}
#endif
