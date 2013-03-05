#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Person.h"
#include "utils.h"

using namespace cv;
using namespace std;

class FaceModel
{
	Ptr<FaceRecognizer> model;
	double threshold;
	int nComponents;
	string name;
	string storageFile;

	void loadImagesFromPeople(vector<Person> people);

public:
#define EIGENFACES 0
#define FISHERFACES 1
#define LBPH 2
	/// @trainnedImages and @trainnedLabels must be keep always in the same order
	vector<Mat> trainnedImages;
	vector<int> trainnedLabels;

	///@modelType must be 0:EINGEFACES, 1:FISHERFACES, 2:LBPH
	FaceModel(int modelType, vector<Person> people, string storageFile="faceRecognizer.xml");
	///@modelType must be 0:EINGEFACES, 1:FISHERFACES, 2:LBPH
	FaceModel(int modelType, vector<Person> people, double threshold, int nComponents, string storageFile="faceRecognizer.xml");
	~FaceModel(void);
	string getName();

	///returns the predicted identity of an image, according to trainnned model
	int identityImage(Mat image);
	///returns the predicted identity of an image, according to trainnned model, in @label and the prediction confidence in @confidence
	void identityImage(Mat image, int& label, double& confidence);
	///returns true if @labelOriginal and predictedLabel are the same and the confidence
	bool isSamePerson(int labelOriginal, Mat image, int& predictedLabel, double& confidence);

	/**Tests a group of peple testing images to verify model accuracy. @return number of correct matches
	* @resultsLabels and @resultsConfidence are predicted results
	*/
	int testModel(vector<Person> people, ofstream& outputfile);
};

