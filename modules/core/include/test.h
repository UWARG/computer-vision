#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "frame.h"

/**
 * @file test.h
 *
 * Abstract class to facilitate testing by handling analyzing results and logging
 */
template <class T, class V>
class Test {
    public:

        /**
         *  @param desc description of the function being tested for the log
         */
        Test(std::string desc): desc(desc) { }

        /**
         *  Runs the test function and logs the results to a file
         *
         *  @param arg argument to be passed to the function being tested 
         *      should be the independant variable of the test
         *  @param desc description of the independant variable for the log
         *  @param expected the value that the function should return
         *  @param iter number of times to run the function
         *      results will be averaged and deviation included with the result
         *  @return the deviation of the results from the expected value
         */
        double do_test(T arg, std::string desc, V expected, int iter = 10) {
            V results[iter];
            for(int i = 0; i < iter; i++){
                results[i] = test(arg);
            }

            V resultMean = mean(results, iter);
            double resultDeviation = deviation(results, expected, iter);
            static const std::string fileName = "results.csv";
            std::ifstream file(fileName.c_str());
            bool fileExists = !file;
            file.close();
            std::ofstream os(fileName.c_str());
            if(!fileExists) {
                os << "date, function, test, expected, mean, deviation" << std::endl;
            }
            std::time_t time = std::time(NULL);
            os << asctime(std::localtime(&time)) << ", "
                << this->desc << ", " << desc << ", " << expected << ", " << resultMean << ", " << resultDeviation << std::endl;
            os.flush();
            os.close();

            return resultDeviation;
        }
    protected:
        /**
         *  The test function 
         */
        virtual V test(T arg) = 0;

        /**
         *  Deviation function for V
         *  @param results the results of running the test function
         *      to be used to calculate the deviation
         *  @param expected the expected value of the results
         *  @param n the number of results
         *  @return the deviation of the results as a double
         */
        virtual double deviation(V results[], V expected, int n) = 0;

        /**
         *  Mean function for V
         *  @param results the results of running the test function
         *      to be averaged
         *  @param n the number of results
         *  @return the average of the results
         */
        virtual V mean(V results[], int n) = 0;
    private:
        std::string desc;
};

#endif
