#include "utils.h"
#include "Library.h"
#include "FaceModel.h"
#include "FaceDetector.h"

void testLibrary(string path, int percetageTrainned, string outputfilename, int m){
	clock_t tStart = clock();
	Library myLib(path, percetageTrainned);	
	ofstream outputfile;
	outputfile.open(outputfilename);

	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;

	tStart = clock();
	FaceModel model(m, myLib.people);
	outputfile << "Trainned model " << model.getName() << " with "<< model.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	model.testModel(myLib.people, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

	outputfile.close();

};

void testLibraryN(string path, int percentageTrainned, string outputfilename, int m, size_t n){
	clock_t tStart = clock();
	Library myLib(path, percentageTrainned);	
	ofstream outputfile, outputfileP, outputfileR;
	outputfile.open("..\\results\\" + outputfilename);

	outputfileP.open("..\\results\\RecognitionCSV\\" + outputfilename);
	outputfileR.open("..\\results\\RetrievalCSV\\" + outputfilename);

	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;

	tStart = clock();
	FaceModel model(m, myLib.people);
	outputfile << "Trainned model " << model.getName() << " with "<< model.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	model.testModelNPredictions(myLib.people, outputfile, outputfileP, n );
	model.testModelPrecision(myLib.people, outputfile, outputfileR, n);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

	outputfile.close();
	outputfileP.close();
	outputfileR.close();

};

void testLoadAndGetTop10(string libPath, int percentageTrainned, int m, string modelFileName, string sample){
	Library myLib(libPath, percentageTrainned);
	FaceModel model(m, modelFileName);

	Mat image = imread(sample, CV_LOAD_IMAGE_GRAYSCALE);
	if(image.rows > 0){
		vector<int> labels;
		vector<double> confidences;
		model.identityImage(image, 10, labels, confidences);

		vector<Person> pp;
		for(size_t i=0; i<labels.size(); i++){
			pp.push_back(myLib.getPerson(labels[i]));

			cout << pp[i].getName() << endl;
		}
	}else{
		cout << "error loading sample" << endl;
	}
}


int faceDetector(int argc, const char *argv[]){
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";

	/// FaceDetector.exe
	bool mask_image = false;
	int normalize_hist = 0;
	if (argc < 3){
		cout << "Usage: path.txt outputDirName [-mask] [-N:(contrast stretching) -E(equalize histogram) -C(CLAHE)] -[-G:(gaussian filter) -B:(bilateral filter)] [-align]" <<endl;
		return -1;
	}else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);
		int filter = 0;
		bool align = false;

		if(argc >= 4){
			string m, n, f, a;
			m = string(argv[3]);
			if(argc >= 5){
				n = string(argv[4]);
			}
			if(argc >=6){
				f = string(argv[5]);
			}
			if(argc >=7){
				a = string(argv[6]);
			}
			if(m == "-mask" || n == "-mask" || f == "-mask"|| a == "-mask"){
				mask_image = true;	
			}
			if(m == "-N" || n == "-N" || f == "-N"|| a == "-N"){
				normalize_hist = ContrastStreatching;	
			}
			if(m == "-E" || n == "-E" || f == "-E"|| a == "-E"){
				normalize_hist = EqualizeHistogram;	
			}
			if(m == "-C" || n == "-C" || f == "-C"|| a == "-C"){
				normalize_hist = CLAHE_Histogram;	
			}
			if(m == "-G" || n == "-G" || f == "-G"|| a == "-G"){
				filter = GaussianFilter;
			}
			if(m == "-B" || n == "-B" || f == "-B" || a == "-B"){
				filter = BilateralFilter;
			}
			if(m == "-align" || n == "-align" || f == "-align" || a == "-align"){
				align = true;
			}
		}

		FaceDetector detectorLPB("..\\helpers\\", "lbpcascade_frontalface.xml", "mask.bmp");
		cout << "detectorLPB -------------------------------------------------------"<< endl<< endl;
		detectorLPB.detectAndCropDir(path, outputfilename, align, mask_image, normalize_hist, filter);
	}
	return 0;
}

int csvCreator(int argc, const char *argv[]){

	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";
	string statsFile = "..\\data\\stats\\default.txt";
	if (argc < 5) {
		cout << "Usage: path.txt outputfilename bottomLimit topLimit [statsFile] " <<endl;
		return -1;
	}
	else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);
		int bottomLimit = atoi(argv[3]);
		int topLimit = atoi(argv[4]);
		vector<int> labelsPerClass;
		vector<string> names;
		if(argc > 5){
			statsFile = "..\\data\\stats\\" + string(argv[5]);
		}
		Library myLib(path, 100);
		cout << myLib.people.size() << endl;
		createCSV(outputfilename, myLib, bottomLimit, topLimit);
		readLfwStats("..\\data\\"+outputfilename, statsFile, labelsPerClass, names);
	}

	return 0;
}

int faceRecognizer(int argc, const char *argv[]){
//int faceRecognizer(int argc, const vector<string>argv){
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";
	int modelType = FISHERFACES;
	if (argc < 3) {
		cout << "Usage: path.txt results.txt [-E(Eingefaces) -F(FisherFaces-default) -L(LBPH)] [nResults]" <<endl;
		return -1;
	}
	else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);
		//ofstream outputfile;
		//outputfile.open(outputfilename);
		if(argc >= 4){
			string m = string(argv[3]);
			if(m == "-E"){
				modelType = EIGENFACES;
			}

			if(m == "-L"){
				modelType = LBPH;
			}
		}

		size_t nResults = 1;
		if(argc >= 5){
			nResults = atoi(argv[4]);
			testLibraryN(path, 80, outputfilename, modelType, nResults);
			//testLoadAndGetTop10(path, 80, modelType, "modelo.xml", 
			//	"C://Users//Pedro//Pictures//lfw//lfw_aligned_BW//Angelina_Jolie//Angelina_Jolie_0005.jpg");
		}else{
			testLibrary(path, 80, outputfilename, modelType);
		}

		cout << "Results in " << outputfilename << endl;
		//outputfile.close();
	}
	

	return 0;
}

int main(int argc, const char *argv[]) {
	/// FaceDetector.exe
	return faceDetector(argc, argv);

	/*
	string path = string(argv[1]);
	string outputfilename = string(argv[2]);
	FaceDetector detectorAlt("..\\helpers\\", "haarcascade_frontalface_alt.xml", "mask.bmp");
	detectorAlt.exportDir(path, outputfilename, ".png");
	*/

	/// CsvCreator.exe
	//return csvCreator(argc, argv);

	/// FaceRecognizer.exe
	//path.txt results.txt [-E(Eingefaces) -F(FisherFaces-default) -L(LBPH)] [nResults]
	//return faceRecognizer(argc, argv);

	//return 0;
}