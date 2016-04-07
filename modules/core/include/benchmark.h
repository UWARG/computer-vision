/**
 * @file benchmark.h
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

#ifndef BENCHMARK_H_INCLUDED
#define BENCHMARK_H_INCLUDED

#include <string>

/**
 *  @brief Does a benchmark of the given function and outputs the results to BOOST_TEST_MESSAGE
 *  @param func Function to be run, should have minimal overhead (ideally setup beforehand and call with a lambda function)
 *  @param iter Number of times the function will be run for profiling. The higher the better.
 */
void benchmark_function(std::string name, std::function<void ()> func, int iter = 100);

#endif // BENCHMARK_H_INCLUDED
