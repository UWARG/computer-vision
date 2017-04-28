/** 
 * @file test.cpp 
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


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TargetAnalysis

#include <boost/test/unit_test.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/property_tree/ptree.hpp>
#include "target_loader.h"

using namespace std;
using namespace boost;

namespace logging = boost::log;

BOOST_AUTO_TEST_CASE(TargetLoaderTest){
    if(boost::unit_test::framework::master_test_suite().argc <= 1) {
        BOOST_ERROR("Invalid number of arguments");
    }
    
    const char* file = boost::unit_test::framework::master_test_suite().argv[1];
    string description = boost::unit_test::framework::master_test_suite().argv[2];    
    TargetLoader loader(file);
    property_tree::ptree* pt = loader.getJSON(); 
    loader.print();
   
}
