#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <string>

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
        Test(std::string desc);

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
        double do_test(T arg, std::string desc, V expected, int iter = 10);
    protected:
        /**
         *  The test function 
         */
        V test(T arg) = 0;

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
