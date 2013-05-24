#include "FaceDetector.h"

FaceDetector::FaceDetector(string dir, string face_cascade_name, string maskName){
	this->dir = dir;
	this->face_cascade_name = dir + face_cascade_name;
	this->mouth_cascade_name = dir + "haarcascade_mcs_mouth.xml";
	this->eyes_cascade_name = dir + "haarcascade_mcs_eyepair_small.xml";
	this->nose_cascade_name = dir + "haarcascade_mcs_nose.xml";
	this->leftEye_cascade_name = dir + "haarcascade_mcs_lefteye.xml";
	this->rightEye_cascade_name = dir + "haarcascade_mcs_righteye.xml";
	this->maskName = dir + maskName;

	//-- 1. Load the FACE cascade
	if( !face_cascade.load( face_cascade_name ) ){ 
		string error_message = "(!)Error loading " + face_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};

	//-- 2. Load the  EYES cascade
	if( !eyes_cascade.load( eyes_cascade_name ) ){ 
		string error_message = "(!)Error loading " + eyes_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};

	if( !leftEye_cascade.load( leftEye_cascade_name ) ){ 
		string error_message = "(!)Error loading " + leftEye_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};
	if( !rightEye_cascade.load( rightEye_cascade_name ) ){ 
		string error_message = "(!)Error loading " + rightEye_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};

	//-- 3. Load the MOUTH cascade
	if( !mouth_cascade.load( mouth_cascade_name ) ){ 
		string error_message = "(!)Error loading " + mouth_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};

	//-- 4. Load the NOSE cascade
	if( !nose_cascade.load( nose_cascade_name ) ){ 
		string error_message = "(!)Error loading " + nose_cascade_name;
		CV_Error(CV_StsBadArg, error_message);
	};

	this->maskImg = imread(this->maskName, CV_LOAD_IMAGE_GRAYSCALE);
	if(!(maskImg.rows>0)){
		string error_message = "(!)Error loading " + this->maskName;
		CV_Error(CV_StsBadArg, error_message);
	}
}


FaceDetector::~FaceDetector(void)
{
}

Mat FaceDetector::applyMask(Mat maskImg, Mat image){
	Mat maskedImage;
	image.copyTo(maskedImage, maskImg);

	return maskedImage;
}

Mat FaceDetector::normalizeConstrast(Mat original, int normalize_hist){
	Mat processedImg;
	Ptr<CLAHE> cl_0;

	switch (normalize_hist){
	case ContrastStreatching:
		normalize(original, processedImg, 0, 255, NORM_MINMAX);
		break;

	case CLAHE_Histogram:
		cl_0 = createCLAHE(2.0, Size(3,3));
		cl_0->apply(original, processedImg);
		break;

	case EqualizeHistogram:
		equalizeHist( original, processedImg);
		break;
	default:
		processedImg = original;
	}

	return processedImg;
}

Mat FaceDetector::applyFilter(Mat original, int filter){
	Mat processedImg = original;

	if(filter == GaussianFilter){
		GaussianBlur( original, processedImg, Size( 3, 3 ), 0, 0 );
	}

	if(filter == BilateralFilter){
		Mat bilateralImg;
		bilateralFilter ( original, bilateralImg, 5, 50, 50 );
		processedImg = bilateralImg;
	}

	return processedImg;
}

Mat FaceDetector::processImg(Mat original, int filter, int normalize_hist){
	Mat processedImg = original;

	processedImg =  normalizeConstrast(original, normalize_hist);

	original = processedImg;

	if(filter > 0){
		processedImg =  applyFilter(original, filter);
	}
	return processedImg;
}


Mat FaceDetector::alignFace(Mat face, Rect myROI){
	Mat original = face;

	face = face(myROI);
	int half_x, half_y, quad_x, qi_quad;
	half_x = face.cols /2;
	quad_x = half_x/2;
	half_y = face.rows /2;
	qi_quad = quad_x/2;

	vector<Rect> leftEyes, rightEyes;
	////-- In each face, detect eyes
	leftEye_cascade.detectMultiScale( face, leftEyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(12, 12) );
	int chosenLeft = -1;
	for( size_t j = 0; j < leftEyes.size(); j++ ){
		if(leftEyes[j].x < quad_x && leftEyes[j].y < half_y && leftEyes[j].width < (half_x+qi_quad) && leftEyes[j].height < half_y){
			if(chosenLeft == -1 || (leftEyes[chosenLeft].width < leftEyes[j].width && leftEyes[j].width < half_x)){
					chosenLeft = j;
			}
		}
	}

	rightEye_cascade.detectMultiScale( face, rightEyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE , Size(12, 12));
	int chosenRight = -1;
	for( size_t j = 0; j < rightEyes.size(); j++ ){
		if(rightEyes[j].x > quad_x && rightEyes[j].x < (half_x+quad_x)  && rightEyes[j].y < half_y && 
			rightEyes[j].width < (half_x+quad_x) && rightEyes[j].height < half_y){
			if(chosenRight == -1 || rightEyes[chosenRight].width < rightEyes[j].width){
					chosenRight = j;
			}
		}
	}
	/*
	if(chosenLeft != -1){
		rectangle(face, leftEyes[chosenLeft], CV_RGB(255, 0,0), 2);
	}

	if(chosenRight != -1){
		rectangle(face, rightEyes[chosenRight], CV_RGB(255, 0,0), 2);
	}
	*/

	// thanks to http://answers.opencv.org/question/497/extract-a-rotatedrect-area/
	Mat M, cropped;
	Mat rotated = face;
	if(chosenLeft != -1 && chosenRight != -1){
		int cx_left,cy_left, cx_right,cy_right;
		cx_left = leftEyes[chosenLeft].x/2;
		cy_left = leftEyes[chosenLeft].y/2;
		cx_right = rightEyes[chosenRight].x/2;
		cy_right = rightEyes[chosenRight].y/2;

		float eye_directionX = float(cx_right - cx_left);
		float eye_directionY = float(cy_right - cy_left);
		
		//# calc rotation angle in radians
		float angle = atan2(eye_directionY,eye_directionX) * 180.0 / PI;

		// rect is the RotatedRect (I got it from a contour...)
		RotatedRect rect(Point2f(((float)original.cols)/2.0,((float)original.rows)/2.0), 
			Size2f((float)original.cols, (float)original.rows), angle);
		
		Size rect_size = rect.size;
		// thanks to http://felix.abecassis.me/2011/10/opencv-rotation-deskewing/
		if (rect.angle < -45.) {
			angle += 90.0;
			swap(rect_size.width, rect_size.height);
		}
		// get the rotation matrix
		M = getRotationMatrix2D(rect.center, angle, 1.0);
		// perform the affine transformation
		warpAffine(original, rotated, M, original.size(), INTER_CUBIC);
	}
	rotated = rotated(myROI);
	/*
	imshow("original", original);
	imshow("rotated", rotated);
	imshow("face", face);
	waitKey(0);
	cout << "---" << endl;
	*/
	return rotated;
}

int FaceDetector::detectAndCrop( Mat frame, string name, string label, string dir, Size size, Size minFeatureSize, bool align, bool apply_mask, int normalize_hist, int filter)
{
	std::vector<Rect> faces, mouth, eyes, nose;
	double verticalCrop = size.height *0.08;
	double horizontalCrop = size.width *0.3;
	int faceIndex = 0, maxFaceRate = 0;
	Mat frame_gray, resizedImg;
	vector<Mat> multipleFaces;
	vector<int> ties, tiesRate;

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
		bool has_eyes = false, has_mouth = false;
		int faceRate = 0;

		//Resize image and crop area of interest
		resizedImg =  frame_gray(faces[i]);
		resize(resizedImg, resizedImg, size);
		//cv::Rect myROI(24, 24, 64, 76);
		int xn = 15;
		int yn = 3;
		cv::Rect myROI(xn, yn, 112-2*xn-2, 112-2*yn-1);
		if(align){
			resizedImg = alignFace(resizedImg, myROI);
		}else{
			resizedImg = resizedImg(myROI);
		}
		
		//in case of multiple faces detection in a sigle image let's investigate more
		if(faces.size() > 1){

			//-- In each face, detect eyes
			eyes_cascade.detectMultiScale( resizedImg, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE , Size(10, 10) );
			if(eyes.size() > 0){
				has_eyes = true;
				faceRate+= 100;
			}
			//-- In each face, detect mouth
			mouth_cascade.detectMultiScale( resizedImg, mouth, 1.1, 2, 0, Size(20, 20) );
			for( size_t k = 0; k < mouth.size(); k++ ){
				if(has_eyes){
					if(abs(eyes[0].y - mouth[k].y) > 20){
						has_mouth = true;
						//rectangle(resizedImg, mouth[k], CV_RGB(0, 255,0), 1);
					}
				}else{
					has_mouth = true;
					//rectangle(resizedImg, mouth[k], CV_RGB(0, 255,0), 1);
				}
			}
			if(has_mouth){
				faceRate+= 50;
			}
			//-- In each face, detect nose
			nose_cascade.detectMultiScale( resizedImg, nose, 1.1, 2, 0, Size(20, 20) );
			if(nose.size() > 0){
				faceRate+=50;
			}

			if(faceRate > maxFaceRate){
				maxFaceRate = faceRate;
				faceIndex = i;
			}else{
				if(faceRate == maxFaceRate){
					ties.push_back(i);
					tiesRate.push_back(faceRate);
				}
			}
			multipleFaces.push_back(resizedImg);
		}
		else{
			multipleFaces.push_back(resizedImg);
		}
	}

	bool has_tie = false;
	if(ties.size() > 0){
		for(size_t t=0; t < ties.size(); t++){
			if(tiesRate[t] == maxFaceRate){
				resizedImg = processImg(multipleFaces[ties[t]], filter, normalize_hist);
				stringstream ss;
				ss << dir<<"\\"<<"VERIFY_"<<t << name;
				cout << ss.str()<< endl;
				outputfileVerify <<"\t VERIFY -> \t" << dir<<"\\" <<name << ";" << label <<endl;
				has_tie = true;

				if(apply_mask){
					imwrite(ss.str(), applyMask(maskImg, resizedImg));
				}else{
					imwrite(ss.str(), resizedImg);
				}
			}//else just ignore that tie because a bigger match was found
		}
	}

	if(faces.size() > 0){
		//apply preprocessing
		resizedImg = processImg(multipleFaces[faceIndex], filter, normalize_hist);
		if(apply_mask){
			imwrite(dir+"\\"+name, applyMask(maskImg, resizedImg));
		}else{
			imwrite(dir+"\\"+name, resizedImg);
		}
		if(!has_tie){
			outputfileClean << dir<<"\\" << name << ";" << label <<endl;
			outputfileVerify << dir<<"\\" << name << ";" << label <<endl;
		}
	}

	return faces.size();
}

//Only optimized for lfw library
int FaceDetector::detectAndCropDir(string path, string outputdir, bool align, bool apply_mask, int normalize_hist, int filter) {
	// DATA
	vector<string> images;
	vector<string> names;
	vector<string> labels;
	vector<string> dirs;
	string outputfilenameclean = outputdir +"\\"+"cleanList.txt";
	string outputfilenameverify = outputdir +"\\"+"withVerify.txt";

	int singleFaces = 0, multipleFaces = 0, noFace = 0;

	loadAndCloneDir(path, outputdir, images, dirs, names, labels);
	outputfileClean.open(outputfilenameclean);
	outputfileVerify.open(outputfilenameverify);

	cout <<endl << "Please check conflicts in files: " <<endl;
	for(unsigned int i = 0; i < names.size(); i++){
		Mat image = imread(images[i], CV_LOAD_IMAGE_COLOR);
		// Smallest face size.
		CvSize minFeatureSize = cvSize(80, 80);
		int n = detectAndCrop(image, names[i], labels[i], outputdir + "\\" + dirs[i] + "\\", Size(112, 112), minFeatureSize, align, apply_mask, normalize_hist, filter);

		switch(n){
		case 1:
			singleFaces++;
			break;
		case 0:
			noFace++;
			break;
		default:
			multipleFaces++;
			break;
		}
	}

	cout << "Sigle Faces: "<< singleFaces << endl;
	cout << "No Face: "<< noFace << endl;
	cout << "Multiple Faces: "<< multipleFaces << endl;

	outputfileVerify << "Sigle Faces: "<< singleFaces << endl;
	outputfileVerify << "No Face: "<< noFace << endl;
	outputfileVerify << "Multiple Faces: "<< multipleFaces << endl;

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

