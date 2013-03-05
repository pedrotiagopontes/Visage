#include "utils.h"
#include "Library.h"
#include "FaceModel.h"
#include "FaceDetector.h"

void testLibrary(string path, int percetageTrainned, ofstream& outputfile){
	clock_t tStart = clock();
	Library myLib(path, percetageTrainned);	
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
};

int main(int argc, const char *argv[]) {
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";

	/*
	/// FaceDetector.exe
	bool mask_image = false;
	if (argc < 3){
		cout << "Usage: path.txt outputDirName [-mask] " <<endl;
	}else{
		path = string(argv[1]);
		outputfilename = string(argv[2]);

		if(argc > 3){
			mask_image = true;	
		}

		FaceDetector detector;
		detector.detectAndCropDir(path, outputfilename, mask_image);
	}
	*/
	

	/*
	/// FaceRecognizer.exe
	if (argc > 1) {
        path = string(argv[1]);
    }

    if (argc == 3) {
		outputfilename = string(argv[2]);
    }

	ofstream outputfile;
	outputfile.open(outputfilename);

	testLibrary(path, 80, outputfile);

	cout << "Results in " << outputfilename << endl;
	outputfile.close();
	*/
	string statsFile = "..\\data\\stats\\default.txt";
	if (argc < 5) {
		cout << "Usage: path.txt outputfilename bottomLimit topLimit [statsFile] " <<endl;
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

	//waitKey(0);
	return 0;
}