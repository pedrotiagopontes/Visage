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
	unsigned int minImagesPerson;
	unsigned int maxImagesPerson;
	int totalImages;
	/// percentage of images per person trainned
	double percentageTrainnedImages;

	/** Creates a new person with @percentageTrainnedImages of @tempImages being added to person 
	* as train images and the rest as test images.
	* The new person is also added to @people
	*/
	void addPerson(int classlabel, string name, string imageDir, vector<string>& tempImages);

	/// Loads a library from a csv file by adding people to @people.	
	void load_csv(const string filename, char separator = ';');

public:
	vector<Person> people;

	Library(string filename, double percentageTrainnedImages = 80.0);
	~Library(void);

	string toString();
	string toStringWithImages();
};

