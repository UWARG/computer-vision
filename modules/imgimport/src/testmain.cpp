#include "pictureimport.cpp"
#include "csvreading.cpp"
#include <iostream>
using namespace std;

int main(){
    vector<int> tem(3);
    PictureImport tester("testcsv.csv","../test",tem);
    return 0;
}
