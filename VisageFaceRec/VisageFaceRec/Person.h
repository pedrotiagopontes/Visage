#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;

class Person
{
	int label;
	string name;
	string imageDir;
	vector<string> trainImages;
	vector<string> testImages;

public:
	Person(int label, string name, string imageDir);
	Person(int label, string name, string imageDir, vector<string> trainImages, vector<string> testImages);
	string toString();
	string toStringWithImages();
	int getLabel();
	string getName();
	string getImageDir();
	vector<string> getTrainImages();
	vector<string> getTestImages();
	void addTrainImage(string image);
	void addTestImage(string image);
};

