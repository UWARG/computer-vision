#include "frame.h"
#include <boost/lexical_cast.hpp>

using namespace cv;
using namespace boost;

int lowThreshold = 60;

frame::frame(Mat src, string ident)
{
    assert(!src.empty());
    this->src = src;
    this->ident = ident;
    this->contours = vector<vector<Point> >();
    this->contourData = vector<c_data>();
}

void frame::run()
{
    log(1, "Filtering Image");
    this->filter_image();
    log(1, "Creating Contours");
    this->create_contours();
    log(1, "Filtering Contours");
    this->filter_contours();
}

/**
 *  filter_image(src) filters the image to emphasize high contrast areas
 */
void frame::filter_image()
{
    CV_Assert(!this->src.empty());

    /*
    Mat result, imgThresholded;
    Mat imgHSV;
    cvtColor(src, imgHSV, COLOR_BGR2HSV);
    */
    /*int error = 5;
    vector<array<int, 3> > averages;
    vector<int> avg_count;

    Mat I = src.clone();
    CV_Assert(I.depth() != sizeof(uchar));
    MatIterator_<Vec3b> it, end;


    Scalar avg = mean(src);
    long total_size = I.rows * I.cols;
    for( it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
    {
        if(isWithin((*it)[0], avg[0], error*10) &&
               isWithin((*it)[1], avg[1], error*10) &&
               isWithin((*it)[2], avg[2], error*10))
        {
            bool found = false;
            for(int i = 0; i < averages.size(); i++)
            {
                if(isWithin((int)(*it)[0], averages[i][0], error) &&
                   isWithin((int)(*it)[1], averages[i][1], error) &&
                   isWithin((int)(*it)[2], averages[i][2], error)  )
                {
                    avg_count[i]++;
                    log(4, "Increasing count: " + lexical_cast<string>(avg_count[i]));
                    found = true;
                }
            }
            if(!found)
            {
                array<int, 3> tmp;
                tmp[0] = (int)(*it)[0];
                tmp[1] = (int)(*it)[1];
                tmp[2] = (int)(*it)[2];
                averages.push_back(tmp);
                avg_count.push_back(1);
                log(4, "Adding " +lexical_cast<string>((int)tmp[0]) + "," +
                    lexical_cast<string>((int)tmp[1]) + "," +
                    lexical_cast<string>((int)tmp[2]) );
            }
        }
    }
    for(int i = 0; i < avg_count.size(); i++)
    {
        if(avg_count[i] < total_size/10)
        {
            avg_count.erase(avg_count.begin() + i);
            averages.erase(averages.begin() + i);
            i--;
        }
    }

    imgThresholded = src.clone();
    log(2, "Thresholding Image");
    for(int i = 0; i < averages.size(); i++)
    {
        inRange(imgThresholded, Scalar(averages[i][0] - error, averages[i][1] - error, averages[i][2] -error),
                   Scalar(averages[i][0] + error, averages[i][1] + error, averages[i][2] + error), imgThresholded);
    }
    cvtColor(imgThresholded, imgThresholded, COLOR_GRAY2BGR);
    multiply(imgThresholded, src, result);
    log(2, lexical_cast<string>(averages.size()) + " Significant Colours");
    log(2, "Merging Image Channels");

    //cvtColor(imgThresholded, result, COLOR_HSV2BGR);

    imshow(process_window, imgThresholded);*/
}

/**
 *  create_contours(src) creates contours using the given image
 */
void frame::create_contours()
{
    this->contours.clear();
    Mat canny_output;
    int ratio = 3;
    int kernel_size = 3;
    int reductionFactor = 2; //beyond 2 seems to decrease effectiveness of the diff'd image
    log(3, "calculating kmeans...");

    Mat tmp;
    resize(src, tmp, Size( src.cols/reductionFactor, src.rows/reductionFactor ));

    /// Analyze colour clusters in image
    Mat samples(tmp.rows * tmp.cols, 3, CV_32F);
    for( int y = 0; y < tmp.rows; y++ )
        for( int x = 0; x < tmp.cols; x++ )
            for( int z = 0; z < 3; z++)
                samples.at<float>(y + x*tmp.rows, z) = tmp.at<Vec3b>(y,x)[z];

    int clusterCount = 10;
    Mat labels;
    int attempts = 1;
    Mat centers;
    kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER, 1, 1), attempts, KMEANS_PP_CENTERS, centers );

    log(3, "Generating new image...");
    Mat new_image( src.size(), src.type() );
    for( int y = 0; y < src.rows; y++ )
        for( int x = 0; x < src.cols; x++ )
        {
            int cluster_idx = labels.at<int>((y/reductionFactor) + (x/reductionFactor)*(src.rows/reductionFactor),0);
            new_image.at<Vec3b>(y,x)[0] = abs(src.at<Vec3b>(y,x)[0] - centers.at<float>(cluster_idx, 0));
            new_image.at<Vec3b>(y,x)[1] = abs(src.at<Vec3b>(y,x)[1] - centers.at<float>(cluster_idx, 1));
            new_image.at<Vec3b>(y,x)[2] = abs(src.at<Vec3b>(y,x)[2] - centers.at<float>(cluster_idx, 2));
        }
    erode(new_image, new_image, 5);
    dilate(new_image, new_image, 5);
    this->diff = new_image;

    /// Reduce noise with a kernel
    blur( new_image, canny_output, Size(kernel_size,kernel_size) );
    blur( new_image, new_image, Size(kernel_size,kernel_size) );
    /// Canny detector
    Canny( canny_output, canny_output, lowThreshold, lowThreshold*ratio, kernel_size );

    log(2, "Detecting Contours");
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    log(2, "Found " + lexical_cast<string>(contours.size()) + " Contours");

    for(int i = 0; i < contours.size(); i++)
    {
        this->contourData.push_back(c_data());
    }

    /*Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    vector<Point> contour_poly;

    /// Draw Contours
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        //approxPolyDP( Mat(contours[i]), contour_poly, 3, true );
        convexHull( Mat(contours[i]), contour_poly, false );
        fillConvexPoly(drawing, contour_poly, color);

        //drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );
    }*/
}

double distance(Point2f *a, Point2f *b)
{
    return sqrt( ((double)a->x - (double)b->x)*((double)a->x - (double)b->x) +
                ((double)a->y - (double)b->y)*((double)a->y - (double)b->y));
}

/**
 *  filter_contours(contours) filters contours to remove unimportant ones
 */
void frame::filter_contours()
{
    log(2, "Filtering Contours " + lexical_cast<string>(contours.size()));
    vector<vector<Point> > tmp;
    vector<Point> contour_poly;
    int orig_size = contours.size();
    Rect boundRect;
    int error = 40;

    for (size_t i = 0; i < contours.size(); i++)
    {
        log(3, "Filtering Contour " + lexical_cast<string>(i));
        //approxPolyDP( Mat(contours[i]), contour_poly, 3, true );
        /// Create convex hull surrounding contour
        convexHull( Mat(contours[i]), contour_poly, false );
        boundRect = boundingRect( Mat(contour_poly) );
        log(3, "Rect is " + lexical_cast<string>(boundRect.tl().x) + "," +
            lexical_cast<string>(boundRect.tl().y) + " " +
            lexical_cast<string>(boundRect.br().x) + "," +
            lexical_cast<string>(boundRect.br().y));
        Mat binaryMat = Mat::zeros(src.size(), CV_8U);

        log(3, "Creating Binary image bounding contour");
        fillConvexPoly(binaryMat, contour_poly, 255);


        int arc_length = arcLength(contour_poly, true);
        double area = contourArea(contour_poly);
        log(3, "Arc: " + lexical_cast<string>(arc_length));
        log(3, "Area: " + lexical_cast<string>(area));
        if((arc_length/5)*(arc_length/5) > area || area < 10)
        {
            this->contours.erase(contours.begin() + i);
            this->contourData.erase(contourData.begin() + i);
            i--;
            continue;
        }
        this->contourData[i].arc_length = arc_length;

        int channels = src.channels();
        int nRows = src.rows;
        int nCols = src.cols;

        int j,k;
        uchar* p;
        uchar* pb;

        uchar prev_b = 0;
        uchar prev_g = 0;
        uchar prev_r = 0;
        double solid = 0;
        double total_size = -1; // -1 so that the first comparison is ignored may induce error
        long total_b = 0;
        long total_g = 0;
        long total_r = 0;
        long total_other_b = 0;
        long total_other_g = 0;
        long total_other_r = 0;
        // Grow rectangle so that it contains surrounding area
        Point p_diff(boundRect.width/10, boundRect.height/10);
        boundRect -= p_diff;
        Size s_diff(boundRect.width/5, boundRect.width/5);
        boundRect += s_diff;

        for( j = boundRect.tl().y >= 0 ? boundRect.tl().y : 0;
            j < boundRect.br().y && j < nRows; ++j)
        {
            p = src.ptr<uchar>(j);
            pb = binaryMat.ptr<uchar>(j);
            for ( k = (boundRect.tl().x >= 0 ? boundRect.tl().x : 0)*channels;
            k < boundRect.br().x*channels && k < nCols*channels; k += channels)
            {
                if(pb[k/channels] == 255) //pixel is inside the contour
                {
                    uchar b = p[k];
                    uchar g = p[k+1];
                    uchar r = p[k+2];
                    log(4, "Comparing: " + lexical_cast<string>((int)b) + " " +
                        lexical_cast<string>((int)prev_b));
                    log(4, "Comparing: " + lexical_cast<string>((int)g) + " " +
                        lexical_cast<string>((int)prev_g));
                    log(4, "Comparing: " + lexical_cast<string>((int)r) + " " +
                        lexical_cast<string>((int)prev_r));
                    if(isWithin((int) b, (int) prev_b, error) &&
                            isWithin((int) g, (int) prev_g, error) &&
                            isWithin((int) r, (int) prev_r, error))
                    {
                        solid++;
                        total_b += b;
                        total_g += g;
                        total_r += r;
                    }
                    total_size++;
                    prev_b = b;
                    prev_g = g;
                    prev_r = r;
                }
                else
                {
                    total_other_b += p[k];
                    total_other_g += p[k+1];
                    total_other_r += p[k+2];
                }
            }
        }
        log(3, "Contour contains " + lexical_cast<string>(solid/total_size) + " solid colour");
        int average_internal_b = total_b/total_size;
        int average_internal_g = total_g/total_size;
        int average_internal_r = total_r/total_size;
        int average_external_b = total_other_b/(boundRect.width * boundRect.height - total_size);
        int average_external_g = total_other_g/(boundRect.width * boundRect.height - total_size);
        int average_external_r = total_other_r/(boundRect.width * boundRect.height - total_size);
        log(3, "Average Internal: " + lexical_cast<string>(average_internal_b) + "," +
            lexical_cast<string>(average_internal_g) + "," +
            lexical_cast<string>(average_internal_r));
        log(3, "Average External: " + lexical_cast<string>(average_external_b) + "," +
            lexical_cast<string>(average_external_g) + "," +
            lexical_cast<string>(average_external_r));
        if(solid/total_size > 1 - error/100.0 && !(isWithin(average_external_b, average_internal_b, error/2)
                && isWithin(average_external_g, average_internal_g, error/2)
                && isWithin(average_external_r, average_internal_r, error/2)))
        {
            log(3, "Accepting contour...");
            contourData[i].average_colour = Scalar(average_internal_b, average_internal_g, average_internal_r);
            contourData[i].area = total_size;
            Moments m = moments(contours[i]);
            contourData[i].centre = Point2f( m.m10/m.m00 , m.m01/m.m00 );
        }
        else
        {
            contours.erase(contours.begin() + i);
            contourData.erase(contourData.begin() + i);
        }
        binaryMat.release();
    }

    /*/// Combine close contours
    vector<vector<Point> > new_contours;
    vector<c_data> new_contour_data;
    for(int i = 0; i < this->contourData.size(); i++)
    {
        c_data tmp = this->contourData[i];
        vector<int> indices;
        for(int j = i + 1; j < this->contourData.size(); j++)
        {
            log(4, "Checking contours " + lexical_cast<string>(tmp.centre.x) + ","
                    + lexical_cast<string>(tmp.centre.y) + " " +
                    lexical_cast<string>(contourData[j].centre.x) + ","
                    + lexical_cast<string>(contourData[j].centre.y)
                     + " at distance: " +
                    lexical_cast<string>(distance(&(tmp.centre),
                                                  &(contourData[j].centre))) + " time: " + ident);
            if(distance(&(tmp.centre), &(contourData[j].centre)) < 1000)
            {
                indices.push_back(j);
                contourData.erase(contourData.begin() + j);
            }
        }
        vector<Point> new_contour;
        for(int j = 0; i < contours[i].size(); j++)
        {

        }
        for(int j = 0; j < indices.size(); j++)
        {
            for(int k = 0; k < contours[j].size(); k++)
            {
                new_contour.push_back(contours[j][k]);
                log(4, "Adding point (distance) " + lexical_cast<string>(contours[j][k].x) + ","
                    + lexical_cast<string>(contours[j][k].y));
            }
        }
        new_contours.push_back(new_contour);
        if(indices.size() > 0)
        {
            log(3, "Combining " + lexical_cast<string>(indices.size() + 1) + " contours");
            convexHull( new_contour, contour_poly, false );
            int arc_length = arcLength(contour_poly, true);
            double area = contourArea(contour_poly);
            Moments m = moments(new_contour);
            Point2f centre( m.m10/m.m00 , m.m01/m.m00 );
            c_data tmp_c_data = {Scalar(0,0,0), centre, area, arc_length}; //TODO: correct colour
            new_contour_data.push_back(tmp_c_data);
        }
        else
        {
            new_contour_data.push_back(contourData[i]);
        }
    }
    this->contours = new_contours;
    this->contourData = new_contour_data;*/

    log(2, "Reduced from " + lexical_cast<string>(orig_size) + " to " + lexical_cast<string>(contours.size()));
}
