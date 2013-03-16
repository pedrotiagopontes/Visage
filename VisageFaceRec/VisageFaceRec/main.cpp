#include "utils.h"
#include "Library.h"
#include "FaceModel.h"
#include "FaceDetector.h"

void testLibrary(string path, int percetageTrainned, ofstream& outputfile){
	clock_t tStart = clock();
	Library myLib(path, percetageTrainned);	
	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;
	/*
	tStart = clock();
	FaceModel modelEIGEN(EIGENFACES, myLib.people);
	outputfile << "Trainned model " << modelEIGEN.getName() << " with "<< modelEIGEN.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelEIGEN.testModel(myLib.people, outputfile);
	*/
	/*
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;
	tStart = clock();
	FaceModel modelFisher(FISHERFACES, myLib.people);
	outputfile << "Trainned model " << modelFisher.getName() << " with "<< modelFisher.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelFisher.testModel(myLib.people, outputfile);
	*/

	outputfile << endl << "-------------------------------------------------------------------------------" << endl;
	tStart = clock();
	FaceModel modelLBPH(LBPH, myLib.people);
	outputfile << "Trainned model " << modelLBPH.getName() << " with "<< modelLBPH.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	modelLBPH.testModel(myLib.people, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

};

void testLibrary(string path, int percetageTrainned, ofstream& outputfile, int m){
	clock_t tStart = clock();
	Library myLib(path, percetageTrainned);	
	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;

	tStart = clock();
	FaceModel model(m, myLib.people);
	outputfile << "Trainned model " << model.getName() << " with "<< model.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	model.testModelNPredictions(myLib.people, outputfile, 2);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

};

void testLibraryN(string path, int percetageTrainned, ofstream& outputfile, int m, size_t n){
	clock_t tStart = clock();
	Library myLib(path, percetageTrainned);	
	outputfile << myLib.toString();
	outputfile << "Loaded in: " << timespent(tStart) << " seconds" << endl <<endl;

	tStart = clock();
	FaceModel model(m, myLib.people);
	outputfile << "Trainned model " << model.getName() << " with "<< model.trainnedImages.size() <<" images in "  << timespent(tStart) << " seconds " << endl;

	model.testModelNPredictions(myLib.people, outputfile, n);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

};

int faceDetector(int argc, const char *argv[]){
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";

	/// FaceDetector.exe
	bool mask_image = false;
	if (argc < 3){
		cout << "Usage: path.txt outputDirName [-mask] " <<endl;
		return -1;
	}else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);

		if(argc > 3){
			mask_image = true;	
		}

		FaceDetector detector;
		detector.detectAndCropDir(path, outputfilename, mask_image);
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
		readLfwStats(path, statsFile, labelsPerClass, names);
		createCSV(outputfilename, labelsPerClass, names, bottomLimit, topLimit);
	}

	return 0;
}

int faceRecognizer(int argc, const char *argv[]){
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";
	int modelType = FISHERFACES;
	if (argc < 3) {
		cout << "Usage: path.txt results.txt [-E(Eingefaces) -F(FisherFaces-default) -L(LBPH)]" <<endl;
		return -1;
	}
	else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);
		ofstream outputfile;
		outputfile.open(outputfilename);
		if(argc == 4){
			string m = string(argv[3]);
			if(m == "-E"){
				modelType = EIGENFACES;
			}

			if(m == "-L"){
				modelType = LBPH;
			}
		}

		//testLibrary(path, 80, outputfile, modelType);
		testLibraryN(path, 80, outputfile, modelType, 3);

		cout << "Results in " << outputfilename << endl;
		outputfile.close();
	}
	

	return 0;
}

int main(int argc, const char *argv[]) {
	/// FaceDetector.exe
	//return faceDetector(argc, argv);

	/// CsvCreator.exe
	//return csvCreator(argc, argv);

	/// FaceRecognizer.exe
	return faceRecognizer(argc, argv);

	//convertImage("C:\\Users\\Pedro\\Pictures\\teste.JPG");
	//return 0;
}