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
	vector<string> trainImages;
	vector<string> testImages;

public:
	Person(int label, string name);
	Person(int label, string name, vector<string> trainImages, vector<string> testImages);
	string toString();
	int getLabel();
	string getName();
	vector<string> getTrainImages();
	vector<string> getTestImages();
	void addTrainImage(string image);
	void addTestImage(string image);
};

