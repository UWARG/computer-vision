/*This file is part of WARG's computer-vision.

    computer-vision is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. You cannot however, claim it as your own. 
	Any code used MUST be referenced to WARG (Waterloo Aerial Robotics Group). 
	Additionally, this code cannot be used in any competition against WARG.

    computer-vision is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with computer-vision.  If not, see <http://www.gnu.org/licenses/>.
	*/
	
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
