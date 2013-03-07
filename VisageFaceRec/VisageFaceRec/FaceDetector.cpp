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

void FaceDetector::detectAndCrop( Mat frame, string name, string dir, Size size, bool apply_mask)
{
	std::vector<Rect> faces;
	Mat frame_gray;
	Mat maskImg = imread(maskName, CV_LOAD_IMAGE_GRAYSCALE);

	if(!(maskImg.rows>0)){
		string error_message = "(!)Error loading " + maskName;
		CV_Error(CV_StsBadArg, error_message);
	}

	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
	if(faces.size() == 0){
		cout << "Couldn't find face in " << dir << name <<endl;
	}
	for( size_t i = 0; i < faces.size(); i++ )
	{
		Mat faceROI = frame_gray( faces[i] );
		std::vector<Rect> eyes;
		//cout << "face detected" << endl;

		//-- Draw the face
		//Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
		//ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
		//rectangle(frame, faces[i],  CV_RGB(0, 255,0), 1);

		Mat resizedImg;
		resize(frame(faces[i]), resizedImg, size);
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
		}else{
			if(apply_mask){
				imwrite(dir+"\\"+name, applyMask(maskImg, resizedImg));
			}else{
				imwrite(dir+"\\"+name, resizedImg);
			}
		}
	}
}

int FaceDetector::detectAndCropDir(string path, string outputdir, bool apply_mask) {
	// DATA
	vector<string> images;
	vector<string> names;
	vector<string> dirs;

	//-- 1. Load the cascade
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	loadAndCloneDir(path, outputdir, images, dirs, names);

	cout <<endl << "Please check conflicts in files: " <<endl;
	for(unsigned int i = 0; i < names.size(); i++){
		Mat image = imread(images[i], CV_LOAD_IMAGE_COLOR);
		detectAndCrop(image, names[i], outputdir + "\\" + dirs[i] + "\\", Size(112, 112), apply_mask);
	}

	return 0;
}

void FaceDetector::loadAndCloneDir(const string& filename, const string& outputname, vector<string>& images, vector<string>& dirs, vector<string>& names, char separator) {
	ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	string path;
	getline(file, path);

	string line, name, pic, lastname="empty";
	int i=0;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, name, '\\');
		getline(liness, pic, separator);
		if(!path.empty()) {
			if(lastname != name){
				string dirname ="mkdir "+ outputname +"\\"+ name;
				system(dirname.c_str());
			}
			images.push_back(path+name+"\\"+pic);
			dirs.push_back(name);
			names.push_back(pic);
			lastname = name;
		}
	}

	cout << "Fineshed cloning dir" << endl;
}
