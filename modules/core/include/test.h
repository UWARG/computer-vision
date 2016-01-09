/**
 * @file test.h
 * Abstract classes to facilitate testing by handling analyzing results and logging
 * @author WARG
 *
 * @section LICENSE
 *
 * Copyright (c) 2015-2016, Waterloo Aerial Robotics Group (WARG)
 * All rights reserved.
 *
 * This software is licensed under a modified version of the BSD 3 clause license
 * that should have been included with this software in a file called COPYING.txt
 * Otherwise it is available at:
 * https://raw.githubusercontent.com/UWARG/computer-vision/master/COPYING.txt
 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iostream>

/**
 * @class Test
 *
 * Complex test class that can be subclassed to wrap a test function that
 * takes arbitrary input and produces arbitrary output
 *
 * Testing is invoked by invoking to_test, which runs the test function and
 * records the results
 */
template <class T, class V>
class Test {
public:

    /**
     * @param desc description of the function being tested for the log
     */
    Test(std::string desc): desc(desc) { }

    /**
     * Runs the test function and logs the results to a file
     *
     * @param arg argument to be passed to the function being tested
     *     should be the independant variable of the test
     * @param desc description of the independant variable for the log
     * @param expected the value that the function should return
     * @param iter number of times to run the function
     *     results will be averaged and deviation included with the result
     * @return the deviation of the results from the expected value
     */
    double do_test(T arg, std::string desc, V expected, int iter) {
        V results[iter];
        for(int i = 0; i < iter; i++){
            results[i] = test(arg);
        }

        V resultMean = mean(results, iter);
        double resultDeviation = deviation(results, expected, iter);
        std::stringstream resultstr;
        resultstr << this->desc << ", " << desc << ", " << expected << ", " << resultMean << ", " << resultDeviation;
        write_test_results(resultstr.str());
        return resultDeviation;
    }
protected:
    /**
     *  The test function
     */
    virtual V test(T arg) = 0;

    /**
     * Deviation function for V
     * @param results the results of running the test function
     *     to be used to calculate the deviation
     * @param expected the expected value of the results
     * @param n the number of results
     * @return the deviation of the results as a double
     */
    virtual double deviation(V results[], V expected, int n) = 0;

    /**
     * Mean function for V
     * @param results the results of running the test function
     *      to be averaged
     * @param n the number of results
     * @return the average of the results
     */
    virtual V mean(V results[], int n) = 0;
private:
    std::string desc;

    /**
     * Writes test results to a file
     * TODO: will eventually be reformatted to XUnit style so Jenkins can parse tests
     * @param formatted string containing comma separated test result data
     */
    void write_test_results(std::string result_string) {
        static const std::string fileName = "results.csv";
        std::ifstream file(fileName.c_str());
        bool fileExists = !file;
        file.close();
        std::ofstream os(fileName.c_str(), std::ios::app);
        if(!fileExists) {
            os << "date, function, test, expected, mean, deviation" << std::endl;
        }
        std::time_t time = std::time(NULL);
        os << asctime(std::localtime(&time)) << ", " << result_string << std::endl;
        os.flush();
        os.close();
    }
};

/**
 * @class SimpleTest
 *
 * Simple boolean result version of Test that implements mean and deviation functions for you
 * It is intended for use with simple tests that should either pass or fail and do so consistently
 * Assumes that the test function always returns the exact same result for any
 * given input, however still should be run multiple iterations to improve accuracy
 * of time tests
 */
template<class T>
class SimpleTest : public Test<T, bool> {
public:
    double do_test(T arg, std::string desc, int iter) {
        Test<T, bool>::do_test(arg, desc, true, iter);
    }

    double do_test(T arg, std::string desc, bool expected, int iter) {
        do_test(arg, desc, iter);
    }

protected:
    double deviation(bool results[], bool expected, int n) {
        return results[0] == expected;
    }

    bool mean(bool results[], int n) {
        return results[1];
    }
};


#endif
