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

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "test.h"
#include "frame.h"
#include <vector>

using namespace cv;
using namespace std;
using namespace boost;
namespace logging = boost::log;

class TargetTest : public Test<Frame &, vector<Point> * > {
    public:
        TargetTest(string s): Test(s) { }

    protected:
        vector<Point> * test(Frame & arg) {
            KMeansFilter filter;
            CannyContourCreator ccreator;
            return NULL;
        }
        
        double deviation(vector<Point> * results[], vector<Point> * expected, int n) {
            return -1;
        }

        vector<Point> * mean(vector<Point> * results[], int n) {
            return NULL;
        }
};

int main(int argc, char ** argv) {
    logging::core::get()->set_filter
    (
       logging::trivial::severity >= logging::trivial::info
    );
    if(argc <= 4) {
        BOOST_LOG_TRIVIAL(info) << "Invalid arguments for test";
        return 1;
    }
    String description = argv[1];
    Mat input = imread(argv[2], cv::IMREAD_COLOR);
    vector<Point> * contour = new vector<Point>();
    for(int i = 3; i + 1 < argc; i++){
        stringstream ss;
        int x, y;
        ss << argv[i];
        ss >> x;
        ss << argv[i+1];
        ss >> y; 
        contour->push_back(Point(x,y));
    }
    Frame f(input, "blah", Metadata());
    TargetTest test("Target Identification using KMeans + Canny");
    double result = test.do_test(f, description, contour);
    return result < 10 && result > -10; // arbitrary bounds for success of test
}
