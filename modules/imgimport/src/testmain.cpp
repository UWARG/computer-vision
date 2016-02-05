#include<iostream>
#include<string.h>
#include<fstream>
#include<dirent.h>
#include"imgimport.cpp"
#include<vector>
using namespace std;
using namespace cv;

int main()
{
	string telemetry_path="./Testcsv.csv";
	string filePath="../test";
	vector<int> v;
	v.push_back(3);
	ImageImport newreader(telemetry_path,filePath,v);
	//cout<<*(newreader.next_frame()->get_img())<<endl;
	return 0;
}
