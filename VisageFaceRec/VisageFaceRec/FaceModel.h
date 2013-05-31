#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

#include "facerec.hpp"

#include "Person.h"
#include "utils.h"

using namespace cv;
using namespace std;

class FaceModel
{
	Ptr<FaceRecognizerExtended> model;
	double threshold;
	int nComponents;
	string name;

	/// @trainnedImages and @trainnedLabels must be keep always in the same order
	vector<Mat> trainnedImages;
	vector<int> trainnedLabels;

	void loadImagesFromPeople(vector<Person> people);

public:
#define EIGENFACES 0
#define FISHERFACES 1
#define LBPH 2

	FaceModel();
	///@modelType must be 0:EINGEFACES, 1:FISHERFACES, 2:LBPH
	FaceModel(int modelType, vector<Person> people);
	///@modelType must be 0:EINGEFACES, 1:FISHERFACES, 2:LBPH
	FaceModel(int modelType, vector<Person> people, double threshold, int nComponents);
	///Building a model from previous existing model;
	FaceModel(int modelType, const string& filename);
	~FaceModel(void);
	string getName();
	int getSizeTrainnedImages();

	///returns the predicted identity of an image, according to trainnned model
	int identityImage(Mat image);
	///returns the predicted identity of an image, according to trainnned model, in @label and the prediction confidence in @confidence
	void identityImage(Mat image, int& label, double& confidence);
	///returns the top @n predictions for a given @image and their confidences in @labels adn  @confidences
	void identityImage(Mat image, size_t n, vector<int> &labels, vector<double> &confidences);
	///returns true if @labelOriginal and predictedLabel are the same and the confidence
	bool isSamePerson(int labelOriginal, Mat image, int& predictedLabel, double& confidence);

	///returns a string array with the top @n persons for a given @image
	vector<Person> topNMatches(string image, size_t n);

	// Serializes this object to a given filename.
	void save(const string& filename) const;
	// Deserializes this object from a given filename.
	void load(const string& filename);
};

