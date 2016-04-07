/**
 * @file benchmark.cpp
 * @author WARG
 *
 * @section LICENSE
 *
 *  Copyright (c) 2015, Waterloo Aerial Robotics Group (WARG)
 *  All rights reserved.
 *
 *  This software is licensed under a modified version of the BSD 3 clause license
 *  that should have been included with this software in a file called COPYING.txt
 *  Otherwise it is available at:
 *  https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#include <boost/test/unit_test.hpp>
#include <string>
#include <chrono>
#include "benchmark.h"

using namespace std;

void benchmark_function(string name, std::function<void ()> func, int iter) {
    auto start = chrono::steady_clock::now();
    for(int i = 0; i < iter; i++) {
        func();
    }
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    BOOST_TEST_MESSAGE("Benchmark of " << name << " took " << (chrono::duration <double, milli>(diff).count()/iter) << " ms per iteration");
}
