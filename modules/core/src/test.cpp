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
double Test<T, V>::do_test(T arg, string desc, V expected, int iter) {
    V results[iter];
    for(int i = 0; i < iter; i++){
        results[i] = test(arg);
    }

    V resultMean = mean(results, iter);
    double resultDeviation = deviation(results, expected, iter);
    static const string fileName = "results.csv";
    ifstream file(fileName);
    bool fileExists = !file;
    file.close();
    ofstream os(fileName);
    if(!fileExists) {
        os << "date, function, test, expected, mean, deviation" << endl;
    }
    std::time_t time = std::time(NULL);
    os << asctime(std::localtime(&time)) << ", " 
        << this->desc << ", " << desc << ", " << expected << ", " << mean << ", " << deviation << endl;
    os.flush();
    os.close();

    return deviation;
}
