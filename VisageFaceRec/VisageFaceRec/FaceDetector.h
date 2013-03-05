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
	//string eyes_cascade_name; // = "haarcascade_eye_tree_eyeglasses.xml";
	string maskName;
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;

	string window_name; // = "Capture - Face detection";

	//RNG rng(12345);

	void loadAndCloneDir(const string& filename, const string& outputname, vector<Mat>& images, vector<string>& dirs, vector<string>& names, char separator=';');

public:
	FaceDetector(string dir = "..\\helpers\\", string face_cascade_name = "lbpcascade_frontalface.xml", string maskName = "mask.bmp");
	~FaceDetector(void);
	Mat applyMask(Mat maskImg, Mat image);
	void detectAndCrop( Mat frame, string name, string dir, Size size, bool apply_mask);
	int detectAndCropDir(string path = "..\\etc\\csv_lfw_10_10_short.txt", string outputdir = "croppedImages", bool apply_mask=true);
};

