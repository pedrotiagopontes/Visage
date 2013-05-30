#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Library.h"
#include "FaceModel.h"

using namespace cv;
using namespace std;

class Evaluator
{
	string libPath;
	Library lib;
	FaceModel model;

	string outputfilename, outputfilenameP, outputfilenameR;

public:
	Evaluator(string path, int percetageTrainned, string outputfilename, int m);
	~Evaluator(void);

	/**Tests a group of peple testing images to verify model accuracy. @return number of correct matches
	* @resultsLabels and @resultsConfidence are predicted results
	*/
	int evaluateModel();
	void evaluateLibraryN(size_t n);
	int evaluateNPredictions( size_t n);
	void evaluatePrecision(size_t threshold);
};

