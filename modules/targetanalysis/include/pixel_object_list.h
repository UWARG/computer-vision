/**
 * @file pixel_object_list.h
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


#ifndef PIXEL_OBJECT_LIST_H_INCLUDED
#define PIXEL_OBJECT_LIST_H_INCLUDED

#include "frame.h"
#include "object.h"
#include "pixel_object.h"
#include <vector>

//Macros and Definitions
#define RAD2DEG(rad) ((rad)*180.0/M_PI)
#define DEG2RAD(deg) ((deg)*M_PI/180.0)
#define EARTH_RADIUS 6371000

/**
 * @class PixelObjectList
 *  
 * PixelObjectList describes a structure for storing pixeltargets and grouping
 * similar ones together. The duplicate and unique forms can then be later found
 * within a unique 'Object' class. The object class contains all pixel objects
 * as well as their averaged data, with accompanying error rates.
 *
 * Module geolocates targets using their pixel locations, visual contour
 * and photo metadata, determines target type and calculates 
 * possible error. As targets are processed unique targets will 
 * be identified and the data combined into a single object.
 *
 * This class is a singleton.
 *
 * @brief Detects unique visual pixel objects and groups them into object based
 * on GPS, visual contour and colour. PixelObjectList is a singleton.
 *
 */

class PixelObjectList{
/*
 * MATCH_THRESHOLD is a fuzzy logic variable, which determines the similarity
 * that a Pixel Object must have to another one in terms of the key attributes
 */

private:
    /**
     * poNode is an instance of a unique object. As part of a linked list, it
     * is linked to other nodes that are deemed also unique. 
     */
    struct poNode{
        Object* o;
        struct poNode* next;

    };

    /**
     * comparitor is a structure used in the processing and sorting of nodes for
     * decisions regarding merging duplicate objects or not.
     */
    struct comparitor{
        double similarity;
        poNode* node;

    };
    /**
     * head is the end of the linked list.
     */
    poNode* head = NULL;
    
     /**
     * tail is the beginning of the linked list.
     */
    poNode* tail = NULL;
    /**
     * The total length of the list is stored in the listLength variable.
     */
    int listLength = 0;
    
    /*
     * firstInstance is a self referencing instance for this singleton class.
     */
    static PixelObjectList* firstInstance;

    /*
     * COMPARE_AREA is a constant (in principle), which at any point can be
     * adjusted. It determines the size of the contours which are compared. It
     * is used to maintain cv::Mat structures with optimal dimensions. Reducing
     * this value is equivalent to reducing the resolution of the contours and
     * would improve computation time.
     */
    int COMPARE_AREA;

    /*
     * Constructor for PixelObjectList
     * 
     * This is a private constructor due to the singleton instance. Use
     * getInstance(), to get or make a new PixelObjectList.
     */
    PixelObjectList(){COMPARE_AREA = 400;};
    
    /*
     * addNode(PixelObject* o) adds a new unique node to the end of the PixelObjectList.
     * @param o the pixelobject that is converted to an object and added to the
     * list.
     */
    void addNode(PixelObject* o);

    /*
     * compareGPS(PixelObject* po1, Object* o2) compares the GPS position of
     * object based on their proximity to one another.
     * @param po1 the pixel object which is being judged and compared against
     * already processed objects.
     * @param o2 the object which has been deemed to have unique parameter in
     * comparison to all other previous objects.
     * @return the result from 0 to 1, based on the similarity of the PO and O.
     */
    double compareGPS(PixelObject* po1, Object* o2);
    
    /*
     * compareColour(PixelObject* po1, Object* o2) compares the average colour value
     * of an object based on previous objects and the criteria for what
     * potential colour may be within the scope of detection.
     * @param po1 the pixel object which is being judged and compared against
     * already processed objects.
     * @param o2 the object which has been deemed to have a unique colour in
     * comparison to all other previous objects.
     * @return the result from 0 to 1, based on the similarity of the PO and O.
     */
    double compareColour(PixelObject* po1, Object* o2);

    /*
     * compareContours(PixelObject* po1, Object* o2) compares the visual outline
     * of an object based on the similarity of the shape and overlap of both
     * objects.
     * @param po1 the pixel object which is being judged and compared against
     * already processed objects.
     * @param o2 the object which has been deemed to have unique contours in
     * comparison to all other previous objects.
     * @return the result from 0 to 1, based on the similarity of the PO and O.
     */ 
    double compareContours(PixelObject* po1, Object* o2);
    
    /*
     * Private operator assignment to prevent mismanagement of singleton
     * instance.
     */
    PixelObjectList& operator=(PixelObjectList*){}; // Private assignment operator
    
    /*
     * Private copy constructor to prevent mismanagement of singleton instance.
     */
    PixelObjectList(PixelObjectList const&){};

    /*
     * Destructor for PixelObjectList.
     */
    ~PixelObjectList();
public:
    /*
     * getInstance() returns the singleton instance of this class. If it is not
     * instantiated it is initialized.
     * @return the singleton PixelObjectList
     */
    static PixelObjectList* getInstance(){
        if (!firstInstance){
            firstInstance = new PixelObjectList;
        }
        return firstInstance;
    }

    /*
     * compareNode(PixelObject* po1, Object* o2) compares po1 to the
     * pixelobjects within o2, however many they may be.
     * @param po1 the pixel object which is being judged and compared against
     * already processed objects.
     * @param o2 the object which has been deemed to have unique parameter in
     * comparison to all other previous objects.
     * @return the result from 0 to 1, based on the similarity of the PO and O.
     */
    double compareNode(PixelObject* po1, Object* o2);
    
    /*
     * addAndCompare(PixelObject* po) iterates the comparison over all known
     * Objects based on the comparison critera. The decision to add a new object
     * to the end of the list, or push it into a duplicate is decided here based
     * on some fuzzy logic.
     * @param po the PixelObject that is being compared and judged.
     */
    void addAndCompare(PixelObject* po);
    
    /*
     * getListLength() is a size getter.
     * @return provides a size value of the current list
     */
    int getListLength();
    
    /*
     * getObject(int index) is a getter for unique objects based on their
     * position in the list.
     * @param index 0-based index for getting unique targets from the list
     * @return a pointer to the unique object requested.
     */
    Object* getObject(int index);
    
    /*
     * getObjects(std::vector<Object*>* v) returns ALL objects.
     * @param v a return parameter to all the objects in the list.
     */
    void getObjects(std::vector<Object*>* v);
};
#endif // PIXEL_OBJECT_LIST_H_INCLUDED
