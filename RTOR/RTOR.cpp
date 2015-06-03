#include "RTOR.h"
#include <boost/lexical_cast.hpp>


using namespace cv;
using namespace std;
using namespace boost;

/// Global Variables
char* image_window = "Image";
char* process_window = "Filtered";
RNG rng(12345);



void init()
{
    // Window for original Image
    namedWindow(image_window, CV_WINDOW_AUTOSIZE);
    // Window for processed image
    namedWindow(process_window, CV_WINDOW_AUTOSIZE);
}

/**
 *  display(src, contours) displays the image and highlights important objects
 */
void display(frame * f)
{
    assert(f);
    assert(!f->src.empty());
    vector<Point> approx;
    for (size_t i = 0; i < f->contours.size(); i++)
    {
        RotatedRect minRect = minAreaRect( Mat(f->contours[i]) );
        approxPolyDP(Mat(f->contours[i]), approx,
                     f->contourData[i].arc_length*0.02, true);
        Rect boundRect = boundingRect( Mat(f->contours[i]) );
        Point p_diff(boundRect.width/10, boundRect.height/10);
        boundRect -= p_diff;
        Size s_diff(boundRect.width/5, boundRect.width/5);
        boundRect += s_diff;
        cv::Mat croppedImage = f->src(boundRect);
        imwrite( "cropped/" + f->ident + lexical_cast<string>(i) , croppedImage );
        if(true)//hierarchy[i][2]<0 && approx.size() > 2)
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            Point2f rect_points[4];
            minRect.points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( f->src, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );
        }
    }
    Mat tmp;
    resize(f->src, tmp, Size( f->src.cols/1.5, f->src.rows/1.5 ));
    imshow(image_window, tmp);
    Mat tmp_2;
    resize(f->diff, tmp_2, Size( f->diff.cols/2, f->diff.rows/2 ));
    imshow(process_window, tmp_2);
}
