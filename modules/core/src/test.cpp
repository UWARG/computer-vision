#include "test.h"
#include <cmath>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

template <class T, class V>
Test<T, V>::Test(string desc):desc(desc) { }

template <class T, class V>
double Test<T, V>::do_test(T arg, string, V expected, int iter) {
    V results[iter];
    double variance, deviation;
    for(int i = 0; i < iter; i++){
        results[i] = test(arg);
    }

    V result_mean = mean(results, iter);
    double result_deviation = deviation(results, expected, iter);

    ofstream os("results.log");
    std::time_t time = std::time(NULL);
    os << asctime(std::localtime(&time)) << " -- " 
        << "Testing " << this->desc << " on " << desc << "; expected: " << expected << "; mean: " << mean << "; deviation: " << deviation << std::endl;
    os.flush();
    os.close();

    return deviation;
}
        
