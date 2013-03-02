#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "opencv2/core/core.hpp"

#include "Person.h"

class Library
{
	string filename;
	string dirPath;
	double percentageTrainnedImages;

	void addPerson(int classlabel, string name, vector<string>& tempImages);
	void load_csv(const string filename, char separator = ';');

public:
	vector<Person> people;

	Library(string filename, double percentageTrainnedImages = 80.0);
	~Library(void);
};

