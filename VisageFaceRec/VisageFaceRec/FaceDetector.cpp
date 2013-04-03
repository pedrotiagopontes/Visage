#include "FaceDetector.h"

FaceDetector::FaceDetector(string dir, string face_cascade_name, string maskName){
	this->dir = dir;
	this->face_cascade_name = dir + face_cascade_name;
	this->maskName = dir + maskName;

	//-- 1. Load the cascade
	if( !face_cascade.load( face_cascade_name ) ){ 
		string error_message = "(!)Error loading " + face_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};
}


FaceDetector::~FaceDetector(void)
{
}

Mat FaceDetector::applyMask(Mat maskImg, Mat image){
	Mat maskedImage;
	image.copyTo(maskedImage, maskImg);

	return maskedImage;
}

Mat FaceDetector::processImg(Mat original, int filter, bool normalize_hist){
	Mat processedImg = original;

	if(normalize_hist){
		equalizeHist( original, processedImg);
		original = processedImg;
	}

	if(filter > 0){
		if(filter == GaussianFilter){
			GaussianBlur( original, processedImg, Size( 1, 1 ), 0, 0 );
		}
		if(filter == BilateralFilter){
			Mat teste;
			bilateralFilter ( original, teste, 5, 100, 100 );
			processedImg = teste;
		}
	}
	return processedImg;
}

void FaceDetector::detectAndCrop( Mat frame, string name, string label, string dir, Size size, Size minFeatureSize, bool apply_mask, bool normalize_hist, int filter)
{
	std::vector<Rect> faces;
	int verticalCrop = size.height *0.2;
	int horizontalCrop = size.width *0.4;
	Mat frame_gray;
	Mat maskImg = imread(maskName, CV_LOAD_IMAGE_GRAYSCALE);
	if(!(maskImg.rows>0)){
		string error_message = "(!)Error loading " + maskName;
		CV_Error(CV_StsBadArg, error_message);
	}
	
	// How detailed should the search be.
	float search_scale_factor = 1.1f;
	// how many neighbors each candidate rectangle should have to retain it.
	int minNeighbors = 3;

	cvtColor( frame, frame_gray, CV_BGR2GRAY );

	//-- Detect faces
	//-- flags are 0 because they are not used in new cascade
	face_cascade.detectMultiScale(frame_gray, faces, search_scale_factor, minNeighbors, 0, minFeatureSize);
	if(faces.size() == 0){
		cout << "\t Couldn't find face in " << dir << name <<endl;
	}
	for( size_t i = 0; i < faces.size(); i++ )
	{
		if(i == 0){
			if(faces.size() == 1){
				outputfileClean <<name << ";" << label <<endl;
				outputfileVerify <<name << ";" << label <<endl;
			}else{
				outputfileVerify <<"\t VERIFY -> \t" <<name << ";" << label <<endl;
			}
		}

		Mat resizedImg =  frame_gray(faces[i]);
		resize(resizedImg, resizedImg, size);
		//cv::Rect myROI(24, 24, 64, 76);
		int xn = 15;
		int yn = 3;
		cv::Rect myROI(xn, yn, 112-2*xn-2, 112-2*yn-1);
		resizedImg = resizedImg(myROI);

		resizedImg = processImg(resizedImg, filter, normalize_hist);
		//imshow( window_name, resizedImg);
		if(i > 0){
			stringstream ss;
			ss << dir<<"\\"<<"VERIFY_"<<i << name;
			cout << ss.str()<< endl;
			if(apply_mask){
				imwrite(ss.str(), applyMask(maskImg, resizedImg));
			}else{
				imwrite(ss.str(), resizedImg);
			}
			outputfileVerify << dir+"\\"+name <<endl;
		}else{
			if(apply_mask){
				imwrite(dir+"\\"+name, applyMask(maskImg, resizedImg));
			}else{
				imwrite(dir+"\\"+name, resizedImg);
			}
		}
	}
}

//Only optimized for lfw library
int FaceDetector::detectAndCropDir(string path, string outputdir, bool apply_mask, bool normalize_hist, int filter) {
	// DATA
	vector<string> images;
	vector<string> names;
	vector<string> labels;
	vector<string> dirs;
	string outputfilenameclean = outputdir +"\\"+"cleanList.txt";
	string outputfilenameverify = outputdir +"\\"+"withVerify.txt";

	//-- 1. Load the cascade
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	loadAndCloneDir(path, outputdir, images, dirs, names, labels);
	outputfileClean.open(outputfilenameclean);
	outputfileVerify.open(outputfilenameverify);

	cout <<endl << "Please check conflicts in files: " <<endl;
	for(unsigned int i = 0; i < names.size(); i++){
		Mat image = imread(images[i], CV_LOAD_IMAGE_COLOR);
		// Smallest face size.
		CvSize minFeatureSize = cvSize(100, 100);
		detectAndCrop(image, names[i], labels[i], outputdir + "\\" + dirs[i] + "\\", Size(112, 112), minFeatureSize, apply_mask, normalize_hist, filter);
	}
	outputfileClean.close();
	outputfileVerify.close();

	return 0;
}

void FaceDetector::exportDir(string path, string outputdir,string extension){
	vector<string> images;
	vector<string> names;
	vector<string> labels;
	vector<string> dirs;

	loadAndCloneDir(path, outputdir, images, dirs, names, labels);
	for(unsigned int i = 0; i < names.size(); i++){
		Mat image = imread(images[i], CV_LOAD_IMAGE_COLOR);
		int pos = names[i].find('.');
		string name = names[i].substr(0, pos);
		imwrite(outputdir + "\\" + dirs[i] + "\\" + name + extension, image);
	}
}

void FaceDetector::loadAndCloneDir(const string& filename, const string& outputname, vector<string>& images, vector<string>& dirs, vector<string>& names, vector<string>&labels, char separator) {
	ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string path, classlabel;
	getline(file, path);
	string line, name, pic, lastname="empty";
	int i=0;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, name, '\\');
		getline(liness, pic, separator);
		getline(liness, classlabel);
		if(!path.empty()) {
			if(lastname != name){
				string dirname ="mkdir "+ outputname +"\\"+ name;
				system(dirname.c_str());
			}
			images.push_back(path+name+"\\"+pic);
			dirs.push_back(name);
			names.push_back(pic);
			labels.push_back(classlabel);
			lastname = name;
		}
	}

	cout << "Fineshed cloning dir" << endl;
}
