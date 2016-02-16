#include "imgimport.cpp"
#include "csvreading.cpp"
#include <iostream>
using namespace std;

int main(){
    vector<int> tem(3);
    ImageImport tester("testcsv.csv","../test",tem);
    return 0;
}
