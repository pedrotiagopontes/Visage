#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

class FaceDetector
{
	string dir; //= "..\helpers\";
	string face_cascade_name; // = "lbpcascade_frontalface.xml";
	string mouth_cascade_name; // = "lbpcascade_frontalface.xml";
	string eyes_cascade_name; // = "haarcascade_eye_tree_eyeglasses.xml";
	string nose_cascade_name; // = "haarcascade_eye_tree_eyeglasses.xml";
	string maskName;
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	CascadeClassifier mouth_cascade;
	CascadeClassifier nose_cascade;

	string window_name; // = "Capture - Face detection";
	ofstream outputfileVerify, outputfileClean;
	
	//RNG rng(12345);

	void loadAndCloneDir(const string& filename, const string& outputname, vector<string>& images, vector<string>& dirs, vector<string>& names, vector<string>&labels, char separator=';');

public:
#define GaussianFilter 1
#define BilateralFilter 2

	FaceDetector(string dir = "..\\helpers\\", string face_cascade_name = "lbpcascade_frontalface.xml", string maskName = "mask.bmp");
	~FaceDetector(void);
	Mat applyMask(Mat maskImg, Mat image);
	Mat processImg(Mat original, int filter=0, bool normalize_hist=true);
	int detectAndCrop( Mat frame, string name, string label, string dir, Size size, Size minFeatureSize, bool apply_mask, bool normalize_hist, int filter);
	int detectAndCropDir(string path, string outputdir = "croppedImages", bool apply_mask=true, bool normalize_hist=false, int filter=0);

	void exportDir(string path, string outputdir,string extension);
};


