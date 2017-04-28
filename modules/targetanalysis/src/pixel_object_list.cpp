/**
 * @file pixel_object_list.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2017, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */


#include "frame.h"
#include "object.h"
#include "pixel_object.h"
#include "pixel_object_list.h"
#include "target_analyzer.h"
#include "contour_comparison.h"
#include "settings.h"
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace boost;
using namespace std;
using namespace cv;

TargetAnalyzerSettings settings = Settings::getInstance()->get_settings().ta;

PixelObjectList* PixelObjectList::firstInstance = NULL;

PixelObjectList::~PixelObjectList(){
    int i = 0;
    poNode* tempPointer;
    while(i<listLength){
        tempPointer = tail;
        tail = tail->next;
        delete(tempPointer);
        i++;
    }
}

void PixelObjectList::addNode(PixelObject* po){
    //Initialize node
    struct poNode* newNode = new struct poNode;
    //Initialize object within node
    Object* newObject = new Object;
    newNode->o = newObject;
        
    newNode->o->add_pobject(po);
    newNode->next = 0; //Nullify pointer (Since there is no next list item)

    //Update old node with the new node
    if (head){  //If HEAD has been initialized already
        head->next = newNode;
    }
    else{ //If HEAD hasn't been initialized already
        tail = newNode;
    }
    //Change head to represent the new node
    head = newNode;
    //Update list length
    listLength++;      
    BOOST_LOG_TRIVIAL(debug) << "Node added. New List Length: " << listLength;
}

double PixelObjectList::compareNode(PixelObject* po1, Object* o2){
    //Use FLANN here?
    //Compare Pixel Objects based on Metadata and visual queues
    //Uses Hierarchical Structure: GPS data is most important, followed by
    //Contour/Shape, followed by Colour
    TargetAnalyzer* ta = TargetAnalyzer::getInstance();
    using TA = TargetAnalyzer; 
    double gps = compareGPS(po1, o2);

    if (gps > settings.GPS_THRESHOLD){//    if (gps > ta->get_threshold(TA::GPS)){
        double visual = compareContours(po1, o2);
        if (visual > ta->get_threshold(TA::CONTOUR)){
            double colour = compareColour(po1, o2);
            if (colour > ta->get_threshold(TA::COLOUR)){
                return (gps + ta->get_threshold_bias(TA::GPS))*(visual + ta->get_threshold_bias(TA::CONTOUR))*(colour + ta->get_threshold_bias(TA::COLOUR)); 
            }
        }
    }
    return 0;
}

double PixelObjectList::compareColour(PixelObject* po1, Object* o2){
    cv::Scalar a = po1->get_colour();
    cv::Scalar b = o2->get_colour();
    double* aArr = a.val;
    double* bArr = b.val;

    double euclideanDistance = sqrt(pow(aArr[0] - bArr[0],2) + pow(aArr[1] -
    bArr[1],2) + pow(aArr[2] - bArr[2],2));

    return 1 - euclideanDistance/sqrt(3*pow(255,2));
}

double PixelObjectList::compareGPS(PixelObject* po1, Object* o2){
    //Calculations assume ideal scenario (no tilt in photos, no distortion)
    Frame* f = po1->get_image();
    cv::Point2d result(0,0);
    if(!TargetAnalyzer::getInstance()->getGPS(po1->get_centroid(),cv::Point2d(69.5/2,125.3/2)/*getSettings()*/,f,&result)){
        //If there is something wrong with the GPS coordinates, its not a valid
        //match
        return 0;
    }
    po1->set_gps_centroid(result);
    double distance = TargetAnalyzer::getInstance()->getGPSDistance(result, o2->get_centroid());
    if (distance < 1){
        distance = 1; //Prevent zero division
    }
    return 1.0/distance; //As you get farther away, probability of the target being the same decreases.       
}


double PixelObjectList::compareContours(PixelObject* po1, Object* o2){
    //Compare width to length ratio, compare area to perimeter ratio, compare
    //square area(widthxlength) to area.
    double maxDiff = sqrt(5);
    
    vector<cv::Point> contour1 = po1->get_contour();
    //Length is the larger dimension
    cv::Rect r1 = cv::boundingRect(contour1);
    double l1 = r1.height;
    double w1 = r1.width;
    //If length is not the larger dimension, switch the values
    if (l1 < w1){
        double temp = l1;
        l1 = w1;
        w1 = temp;
    }
    double w_l_ratio1 = w1/l1;
    double a_p_ratio1 = po1->get_area()/po1->get_perimeter();
    double sq_a_ratio1 = po1->get_area()/(l1*w1);

    for (PixelObject* po2 : o2->get_pobjects()){
        vector<cv::Point> contour2 = po2->get_contour();

        cv::Rect r2 = cv::boundingRect(contour2);
        double l2 = r2.height;
        double w2 = r2.width;
        if (l2 < w2){
            double temp = l2;
            l2 = w2;
            w2 = temp;
        } 
        double w_l_ratio2 = w2/l2;
        double a_p_ratio2 = po2->get_area()/po2->get_perimeter();
        double sq_a_ratio2 = po2->get_area()/(l2*w2);

        double diff = sqrt(pow((l1 - l2)/l1, 2) + pow((w1 - w2)/w2,2) +
        pow((w_l_ratio1 - w_l_ratio2)/w_l_ratio1,2) + pow((a_p_ratio1 -
        a_p_ratio2)/a_p_ratio1,2) + pow((sq_a_ratio1 -
        sq_a_ratio2)/sq_a_ratio1,2)); //5-dimensions -> Max value is sqrt(5)

        if (diff < maxDiff){
            maxDiff = diff;
        }
    }
    return 1 - maxDiff/sqrt(5);
}

/*double PixelObjectList::compareContours(PixelObject* po1, Object* o2){
    const vector<PixelObject*>& poList = o2->get_pobjects();
        
    double maxSimilarity = 0;
    vector<cv::Point> minimumContour;
    for (PixelObject* po2 : poList){
        vector<cv::Point> v1 = po1->get_contour();
        vector<cv::Point> v2 = po2->get_contour();
        Point c1 = po1->get_centroid();
        Point c2 = po2->get_centroid();

        //TODO: Choose A1, A2, po1, po2 based on the smaller area...saves time on computation
        int a1 = po1->get_area();
        int a2 = po2->get_area();
            
        if (a1 == 0 || a2 == 0){
            continue;
        }

        //cArea determines size and accuracy of the matching. Should be at
        //least 100.
        double areaScale1 = (double)COMPARE_AREA/a1;
        double areaScale2 = (double)COMPARE_AREA/a2;

        //Rotate different Angles
        int numIntervals = 36; //Should vary based on computation time requirements
        vector<vector<cv::Point>> v2Mod; 
        vector<cv::Point> v1Mod;
        
        //Center the contour at (0,0)
        for (Point i : v1){
            v1Mod.push_back(areaScale1*(i - c1));
        }

        //Center the contour at (0,0) and rotate it and scale it
        for (int interval = 0; interval < numIntervals; interval++){
            double theta = 2*M_PI/numIntervals * interval;
            BOOST_LOG_TRIVIAL(debug) << "theta: " << theta << ", aS2: " <<
            areaScale2 << ", C2: " << c2;

            vector<cv::Point> tempVec;
            for (Point i : v2){
                tempVec.push_back(Point(areaScale2*cos(theta)*(i.x-c2.x) -
                areaScale2*sin(theta)*(i.y-c2.y),
                areaScale2*sin(theta)*(i.x-c2.x) + areaScale2*cos(theta)*(i.y-c2.y)));
            }    
            v2Mod.push_back(tempVec);
        }
            
        float radius1, radius2;
        Point2f center1, center2;
        minEnclosingCircle(v1Mod,center1,radius1);
        minEnclosingCircle(v2Mod[0],center2,radius2);
        //If areas are similar, check size. If its way off (factor of 2), move onto next iteration.
        if (abs(radius1 - radius2) > radius1||abs(radius1 - radius2) > radius2){
            //Skip analysis to save computation time
            continue;
        }

        for (int i = 0; i < numIntervals; i++){
            //Reposition the contours for Mat operations (in the positive quadrant)
            Point matReference(radius1-center1.x > radius2-center2.x?radius1-center1.x:radius2-center2.x, radius1-center1.y > radius2-center2.y?radius1-center1.y:radius2-center2.y);
            for (int j = 0; j < v1Mod.size();j++){
                v1Mod[j] = v1Mod[j] + matReference;
            }
            for (int j = 0; j < v2Mod[i].size(); j++){
                v2Mod[i][j] = v2Mod[i][j] + matReference;
            }
            vector<vector<Point>> contoursWrapperA, contoursWrapperB;
            contoursWrapperA.push_back(v1Mod);
            contoursWrapperB.push_back(v2Mod[i]);
                
            //No point comparing contours if there is only one.
            if (listLength >= 1){
                double sim = compare_contours(contoursWrapperA, contoursWrapperB);
                if (maxSimilarity < sim){
                    BOOST_LOG_TRIVIAL(debug) << "Found new optimized position: " << sim;
                    maxSimilarity = sim;
                    minimumContour = v2Mod[i];
                        
                  */ /*Visualization
                    if (sim > 0.7){
                        Mat drawing = Mat::zeros(matReference.x*2, matReference.y*2, CV_8UC3);

                        drawContours(drawing, contoursWrapperA, 0, cv::Scalar(255,0,255), FILLED);
                        drawContours(drawing, contoursWrapperB, 0, cv::Scalar(0,255,255), FILLED);
 
                        namedWindow( "Duplicate Contours", CV_WINDOW_AUTOSIZE );
                        imshow( "Duplicate Contours", drawing );
                        waitKey(0);
                    }*//*   
                }
            }
        }
    } 
    return maxSimilarity;
}*/

void PixelObjectList::addAndCompare(PixelObject* po){
    //Iterate over list
    int i = 0;
    struct comparitor listMatch[listLength];
    double maxSimilarity = 0;
    int iMax = 0;

    poNode* tempPointer;
    tempPointer = tail;
    BOOST_LOG_TRIVIAL(info) << "List Length: " << listLength;
    while(i<listLength){
        //Returns similarity percentage - 1 is an ideal match, 0 is the worst
        //match possible
        listMatch[i].similarity = compareNode(po, tempPointer->o);
        listMatch[i].node = tempPointer;

        //Update the best match if a better one is available
        if (listMatch[i].similarity > maxSimilarity){
            maxSimilarity = listMatch[i].similarity;
            iMax = i;
        }
        i++;
        tempPointer = tempPointer->next;
    }
    //Is the best match good enough - FUZZY LOGIC
    if (maxSimilarity >= TargetAnalyzer::getInstance()->get_threshold(TargetAnalyzer::MATCH)){
        //The add_pobject function should also recalculate all parameters of the
        //object.
        listMatch[iMax].node->o->add_pobject(po);
    }
    else{
        //Add the node to the end of the list for future comparisons
        //Note, that objects that have already matched are not included for
        //future comparisons. This needs to be tested in the field.    
        addNode(po);
    }

}

int PixelObjectList::getListLength(){
    return listLength;
}

Object* PixelObjectList::getObject(int index){
    struct poNode* node = tail; 
    for (int i = 0; i < index; i++){
        node = node->next;
    }
    return node->o;
}

void PixelObjectList::getObjects(vector<Object*>* v){
    struct poNode* node = tail;
    for (int i = 0; i < listLength; i++){
        v->push_back(node->o);
        node = node->next;
    }
}
