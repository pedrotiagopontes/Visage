#include "utils.h"
#include "algorithmsTest.h"

#include "Library.h"
#include "FaceModel.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/** Global variables */
string face_cascade_name = "lbpcascade_frontalface.xml";
string eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
Mat maskImg = imread("mask.bmp", CV_LOAD_IMAGE_GRAYSCALE);

RNG rng(12345);

Mat applyMask(Mat maskImg, Mat image){
	Mat maskedImage;
	image.copyTo(maskedImage, maskImg);

	return maskedImage;
}

void detectAndCrop( Mat frame, string name, string dir, Size size)
{
	vector<Mat> croppedImages;
   std::vector<Rect> faces;
   Mat frame_gray;

   cvtColor( frame, frame_gray, CV_BGR2GRAY );
   equalizeHist( frame_gray, frame_gray );

   //-- Detect faces
   face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );

   for( size_t i = 0; i < faces.size(); i++ )
    { Mat faceROI = frame_gray( faces[i] );
      std::vector<Rect> eyes;
	  //cout << "face detected" << endl;

	  //-- Draw the face
      //Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
      //ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
	  //rectangle(frame, faces[i],  CV_RGB(0, 255,0), 1);

	  //-- Show what you got
	  Mat resizedImg; // = frame(faces[i]);
	  resize(frame(faces[i]), resizedImg, size);
	  //imshow( window_name, resizedImg);
	  if(i > 0){
		  stringstream ss;
		  ss << dir<<"\\"<<"VERIFY_"<<i << name;
		  cout << ss.str()<< endl;
		  imwrite(ss.str(), applyMask(maskImg, resizedImg));
	  }else{
		imwrite(dir+"\\"+name, applyMask(maskImg, resizedImg));
	  }
	  //waitKey(0);
    }
}

int detectAndCropDir(string path = "..\\etc\\csv_lfw_10_10_short.txt", string outputdir = "croppedImages") {
    // DATA
	vector<Mat> images;
	vector<string> names;
	vector<string> dirs;

	 //-- 1. Load the cascade
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	read_and_createDir(path, outputdir, images, dirs, names);
	Size imgSize = images[0].size();
	for(unsigned int i = 0; i < images.size(); i++){
		detectAndCrop(images[i], names[i], outputdir + "\\" + dirs[i] + "\\", Size(112, 112));
	}

	return 0;
}

int main(int argc, const char *argv[]) {
    //string att_path = "..\\etc\\at.txt";
    //string lfw_color_path = "..\\etc\\lfw_color.txt";
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";

	if (argc > 1) {
        path = string(argv[1]);
    }

    if (argc == 3) {
		outputfilename = string(argv[2]);
    }
	ofstream outputfile;
	outputfile.open(outputfilename);
	
	//algorithmsTest(path, outputfilename);
	//detectAndCropDir(path, "maskedImages");

	/*
	vector<int> originalLabels;
	vector<string> names;
	vector<int> testResults;
	vector<double> testConfidence;
	*/

	clock_t tStart = clock();
	Library myLib(path, 80);	
	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;

	tStart = clock();
	FaceModel modelEIGEN(EIGENFACES, myLib.people);
	outputfile << "Trainned model " << modelEIGEN.getName() << " with "<< modelEIGEN.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelEIGEN.testModel(myLib.people, outputfile);

	outputfile << endl << "-------------------------------------------------------------------------------" << endl;
	tStart = clock();
	FaceModel modelFisher(FISHERFACES, myLib.people);
	outputfile << "Trainned model " << modelFisher.getName() << " with "<< modelEIGEN.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelFisher.testModel(myLib.people, outputfile);

	outputfile << endl << "-------------------------------------------------------------------------------" << endl;
	tStart = clock();
	FaceModel modelLBPH(LBPH, myLib.people);
	outputfile << "Trainned model " << modelLBPH.getName() << " with "<< modelFisher.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelLBPH.testModel(myLib.people, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;
	
	cout << "Results in " << outputfilename << endl;
	outputfile.close();

	//waitKey(0);
	return 0;
}