/**
 * @file object_list.h
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */


#include "frame.h"
#include "object.h"
#include "object_list.h"

ObjectList::ObjectList(){
    listLength = 0;
    tail = head;
}

ObjectList::~ObjectList(){
    int i = 0;
    poNode* tempPointer;
    while(i<listLength){
        tempPointer = tail;
        tail = tail->next;
        delete(tempPointer);
        i++;
    }
}

bool ObjectList::addNode(Object* o){
    struct oNode* newNode = new struct oNode;
    if (newNode){
        newNode->o = o;
        newNode->next = 0; //Nullify pointer (Since there is no next list item)
        //Update old node with the new node
        head->next = newNode;
        //Change head to represent the new node
        head = newNode;
        //Update list length
        listLength++;
        return true;
    }

    return false;

}

bool ObjectList::getGPSDuplicates(){
    //TODO: Consider changing the return type.
    //Should return array of probabilities (% error based on known GPS error and
    //other parameters)
    //After multiplying by the visual probability, a good estimate of whether or
    //not it is a duplicate will be known
    int i = 0;
    while(i < listLength){
        tail->o->
        i++;
    }
    return true;
}

//TODO:Add later to compare duplicate targets from a visual perspective
//bool ObjectList::getVisualDuplicates(){
    
//}
