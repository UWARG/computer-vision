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
    Mat input = imread(argv[2], CV_LOAD_IMAGE_COLOR);
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
