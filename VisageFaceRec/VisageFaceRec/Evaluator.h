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

/** Class created to evalute the perfomance of a face recognition model and algorithm in a specific library */
class Evaluator
{
	string libPath;
	Library lib;
	FaceModel model;
	string outputfilename, outputfilenameP, outputfilenameR;
	
	/** Evalutes the rank n performance of a model and algorithm in a specific library, produces two files:
	* - In folder "results", file "outputfilename": human readeble format
	* - In folder "results/RecognitionCSV", file "outputfilename": semicolon separated values
	*		1st line: recognition % grouped per person
	*		2nd line: Acumulated % grouped per person
	*		3rd line: % grouped per image
	*		4th line: Acumulated % grouped per image
	*/
	int evaluateNPredictions( size_t n);

	/** Evalutes the performance of a model and algorithm in a specific library in an retrival precision perspective
	* produces two files:
	* - In folder "results", file "outputfilename": human readeble format
	* - In folder "results/RetrievalCSV", file "outputfilename": semicolon separated values
	*		1st line: AVG % grouped per person
	*		2nd line: AVG % grouped per image
	*/
	void evaluatePrecision(size_t threshold);

public:
	Evaluator(string path, int percetageTrainned, string outputfilename, int m);
	~Evaluator(void);

	/** Evalutes the rank 1 performance of a model and algorithm in a specific library, produces human
	* reable outputfile in results */
	int evaluate();

	/** Evalutes the performance of a model and algorithm in a specific library 
	* by calling evaluateNPredictions and evaluatePrecision */
	void evaluateN(size_t n);
};

