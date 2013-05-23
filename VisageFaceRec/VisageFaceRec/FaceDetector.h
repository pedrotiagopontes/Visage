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
#define PI 3.141592653589793238462643383279502884

	string dir; //= "..\helpers\";
	string face_cascade_name; // = "lbpcascade_frontalface.xml";
	string mouth_cascade_name; // = "lbpcascade_frontalface.xml";
	string eyes_cascade_name; // = "haarcascade_eye_tree_eyeglasses.xml";
	string nose_cascade_name; // = "haarcascade_eye_tree_eyeglasses.xml";
	string rightEye_cascade_name; // = "haarcascade_mcs_righteye.xml";
	string leftEye_cascade_name; // = "haarcascade_mcs_lefteye.xml";
	string maskName;
	Mat maskImg;
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	CascadeClassifier mouth_cascade;
	CascadeClassifier nose_cascade;
	CascadeClassifier leftEye_cascade;
	CascadeClassifier rightEye_cascade;

	string window_name; // = "Capture - Face detection";
	ofstream outputfileVerify, outputfileClean;

	void loadAndCloneDir(const string& filename, const string& outputname, vector<string>& images, vector<string>& dirs, vector<string>& names, vector<string>&labels, char separator=';');

public:
#define GaussianFilter 1
#define BilateralFilter 2
#define ContrastStreatching 1
#define EqualizeHistogram 2
#define CLAHE_Histogram 3

	FaceDetector(string dir = "..\\helpers\\", string face_cascade_name = "lbpcascade_frontalface.xml", string maskName = "mask.bmp");
	~FaceDetector(void);
	Mat applyMask(Mat maskImg, Mat image);
	Mat applyFilter(Mat original, int filter);
	Mat normalizeConstrast(Mat original, int normalize_hist);
	Mat processImg(Mat original, int filter=0, int normalize_hist = EqualizeHistogram);
	int detectAndCrop( Mat frame, string name, string label, string dir, Size size, Size minFeatureSize, bool align, bool apply_mask, int normalize_hist, int filter);
	int detectAndCropDir(string path, string outputdir = "croppedImages", bool align = false, bool apply_mask=true, int normalize_hist=EqualizeHistogram, int filter=0);
	Mat alignFace(Mat face, Rect myROI);

	void exportDir(string path, string outputdir,string extension);
};
