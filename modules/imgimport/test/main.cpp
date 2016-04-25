/*
 * main.cpp - Implementation of the DeckLinkCapture class
 *
 *   Copyright 2013 Jesús Torres <jmtorres@ull.es>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <iostream>
#include <utility>
#include <vector>

#include <errno.h>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#ifdef HAS_DECKLINK
#include <DeckLinkAPI.h>

#include "ComPtr.h"
#include "DeckLinkCapture.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/property_tree/ptree.hpp>

using namespace boost;

namespace logging = boost::log;

int testResult = 1;

int main()
{
    ComPtr<IDeckLinkIterator> deckLinkIterator = CreateDeckLinkIteratorInstance();
    if (! deckLinkIterator) {
        BOOST_LOG_TRIVIAL(error) << "This application requires a DeckLink driver. \n";
        return 1;
    }

    std::vector<DeckLinkCapture> captures;
    std::vector<std::string> windows;

    IDeckLink* deckLink;

    while (deckLinkIterator->Next(&deckLink) == S_OK) {
        captures.push_back(std::move(DeckLinkCapture(ComPtr<IDeckLink>(deckLink))));

        std::string windowName = boost::str(boost::format("%s <%i>") %
            captures.back().getDeviceDisplayName() % captures.size());

        cv::namedWindow(windowName);

        windows.push_back(windowName);
    }

    if (captures.size() == 0) {
        BOOST_LOG_TRIVIAL(error) << program_invocation_short_name
                  		 << ": No DeckLink device was found"
                  		 << std::endl;
        return 2;
    }

    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        if (! capture.start())
            BOOST_LOG_TRIVIAL(error) << program_invocation_short_name
                      		     << ": Could not start the capture on the device.'"
                      		     << capture.getDeviceDisplayName()
                      		     << "': "
                      		     << capture.errorString()
                      		     << std::endl;
    }

    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        capture.grab();
    }
    for(unsigned i = 0; i < captures.size(); ++i) {
        cv::Mat frame;
        captures[i].retrieve(frame);
        if (frame.rows > 0){
	    BOOST_LOG_TRIVIAL(info) << "Frame Rows: " << frame.rows;
            BOOST_LOG_TRIVIAL(info) << "Frame Columns: " << frame.cols;
            testResult = 0; //The test was sucessful if it entered the 'if' statement.  
	}        
        //cv::imshow(windows[i], frame);
    }
   
    BOOST_FOREACH(DeckLinkCapture& capture, captures)
    {
        capture.stop();
    }

    //cv::destroyAllWindows();

    return testResult;
}
#endif
