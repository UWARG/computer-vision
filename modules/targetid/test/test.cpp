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
#include <vector>
#include <cmath>
#include <iostream>
#include <climits>
#include "target_identifier.h"
#include "canny_contour_creator.h"
#include "k_means_filter.h"
#include "test.h"
#include "frame.h"

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
            Mat * filtered = filter.filter(arg.get_img());
            vector<vector<Point> > * results = ccreator.get_contours(*filtered);
            vector<Point> * result = NULL;
            if(results->size() > 0) {
                result = new vector<Point>(results->at(0));
            }
            return result;
        }
        
        double deviation(vector<Point> * results[], vector<Point> * expected, int n) {
            double variance = 0;
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < expected->size(); j++){
                    if(results[i] == NULL) continue;
                    variance += closest_dist(results[i], expected->at(j));
                }
            }
            variance /= n;
            return sqrt(variance);
        }

        double closest_dist(vector<Point> * src, Point & ref) {
            double minDist = INT_MAX;
            int index = -1;
            for(int i = 0; i < src->size(); i++) {
                double dist = distance(ref, src->at(i));
                if(dist < minDist) {
                    minDist = dist;
                    index = i;
                }
            }
            return minDist;
        }

        double distance(Point & p1, Point & p2) {
            return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
        }

        vector<Point> * mean(vector<Point> * results[], int n) {
            vector<Point> * mean = new vector<Point>(n);
            for(int i = 0; i < n; i++) {
                vector<Point> * result = results[i];
                if(result == NULL) continue;
                for(int j = 0; j < result->size(); j++) {
                    mean->assign(i, Point(result->at(j).x/n, result->at(j).y/n));
                }
            }
            return mean;
        }
};

ostream & operator<<(ostream & out, vector<Point> * contour) {
    for(int i = 0; i < contour->size(); i++) {
        out << contour->at(i);
        //if(i < contour->size() - 1) out << " ";
    }
    return out;
}

int main(int argc, char ** argv) {
    logging::core::get()->set_filter
    (
       logging::trivial::severity >= logging::trivial::info
    );
    if(argc <= 4) {
        BOOST_LOG_TRIVIAL(info) << "Invalid arguments for test";
        return 2;
    }
    string description = argv[1];
    Mat input = imread(argv[2], cv::IMREAD_COLOR);
    BOOST_LOG_TRIVIAL(info) << "Reading " << argv[2] << " of size " << input.rows << "x" << input.cols;
    vector<Point> * contour = new vector<Point>();
    for(int i = 3; i + 1 < argc; i++){
        stringstream ss;
        int x, y;
        ss << argv[i] << " " << argv[i+1];
        ss >> x >> y;
        contour->push_back(Point(x,y));
    }
    BOOST_LOG_TRIVIAL(info) << "Read Contour: " << contour; // TODO: figure out why defining operator<< doesn't affect boost logs
    Frame f(&input, "blah", Metadata());
    TargetTest test("Target Identification using KMeans + Canny");
    double result = test.do_test(f, description, contour);
    return !(result < 10 && result > -10); // arbitrary bounds for success of test (false indicates success)
}
