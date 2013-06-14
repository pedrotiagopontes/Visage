#include "utils.h"
#include "Library.h"
#include "FaceModel.h"
#include "Evaluator.h"
#include "FaceDetector.h"

void loadAndGetTop(string requestId, string libPath, int percentageTrainned, int m, string modelFileName, string sample, int n){
	Library myLib(libPath, percentageTrainned);
	FaceModel model(m, modelFileName);
	FaceDetector detector("..\\helpers\\", "lbpcascade_frontalface.xml", "mask.bmp");

	ofstream outfile;
	outfile.open("topN\\" + requestId);
	
	Mat image = imread(sample, CV_LOAD_IMAGE_COLOR);
	if(image.rows > 0){
		Mat faceDetected = detector.detectAndCropDefault(image);
		
		vector<int> labels;
		vector<double> confidences;
		model.identityImage(faceDetected, n, labels, confidences);
		
		cout << endl << "Imagem: " << sample << endl << endl;
		vector<Person> pp;
		for(size_t i=0; i<labels.size(); i++){
			pp.push_back(myLib.getPerson(labels[i]));
			cout << "  " << i+1 << ": " << pp[i].getName() << endl;
			outfile << pp[i].getName() << ";";
			outfile << pp[i].getImageDir() << pp[i].getName() << "\\" << endl;
		}
		cout <<endl;
		
	}else{
		outfile << "ERROR";
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

int faceRecognizer(int argc, const char *argv[]){
//int faceRecognizer(int argc, const vector<string>argv){
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";
	string m = "";
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
		Evaluator ev = Evaluator(path, 80, outputfilename, modelType);
		size_t nResults = 1;
		if(argc >= 5){
			nResults = atoi(argv[4]);
			ev.evaluateN(nResults);
		}else{
			ev.evaluate();
		}

		if(argc >= 6)
		cout << "Results in " << outputfilename << endl;
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

int trainAndSaveModel(int argc, const char *argv[]){
	string path = "..\\etc\\original.txt";
	string percent = "100";
	string m = "-L";
	string outputfilename = "modelo.xml";

	int modelType = LBPH;
	int percent_i = 100;

	if (argc < 5) {
		cout << "Usage: libraryPath.txt percentageToTrain [-E(Eingefaces) || -F(FisherFaces-default) || -L(LBPH)] output.xml" <<endl;
		return -1;
	}
	else{
		path = string(argv[1]);
		percent = string(argv[2]);
		m = string(argv[3]);
		outputfilename = string(argv[4]);

		if(m == "-E"){
			modelType = EIGENFACES;
		}
		if(m == "-F"){
			modelType = FISHERFACES;
		}
		
		percent_i = atoi(percent.c_str());
	}
	Library lib = Library(path, percent_i);
	FaceModel model = FaceModel(modelType, lib.people);
	model.save(outputfilename);

	cout <<"Trainned model for library: " << path << " using " << percent_i << "% of the lib images" << endl;
	cout <<"Saved library face model in: saved\\" << outputfilename << endl;

	createLoadFile(outputfilename, path, percent_i, modelType); 

	return 0;
}

int topN(int argc, const char *argv[]){
	string libPath = "";
	string imagePath = "";
	string model = "model.xml";
	string nResults = "10";
	string modelType = "2";
	string percent = "80";
	string requestId = "default";
	if (argc < 5) {
		cout << "Usage: requestId imagePath model.xml nResults" <<endl;
		return -1;
	}
	else{
		requestId = string(argv[1]);
		imagePath = string(argv[2]);
		model = string(argv[3]);
		nResults = string(argv[4]);

		ifstream loadFile;
		loadFile.open("..\\saved\\" + model + ".visage");

		loadFile >> libPath;
		loadFile >> percent;
		loadFile >> modelType;
		loadFile >> model;

		loadAndGetTop(requestId, libPath, atoi(percent.c_str()), atoi(modelType.c_str()), model, imagePath, atoi(nResults.c_str()) );
	}
	return 0;
}

int exportDir(int argc, const char *argv[]){
	FaceDetector detectorLPB("..\\helpers\\", "lbpcascade_frontalface.xml", "mask.bmp");
	if(argc > 3){
		detectorLPB.exportDir(argv[1], argv[2], argv[3]);
	}else{
		cout << "Usage: dirPath outputdir extension" <<endl;
		return -1;
	}
}

int maskDir(int argc, const char *argv[]){
	FaceDetector detectorLPB("..\\helpers\\", "lbpcascade_frontalface.xml", "mask.bmp");
	if(argc > 2){
		detectorLPB.maskDir(argv[1], argv[2]);
	}else{
		cout << "Usage: dirPath outputdir" <<endl;
		return -1;
	}
}

int main(int argc, const char *argv[]) {
	/// FaceDetector.exe
	//return faceDetector(argc, argv);

	/// CsvCreator.exe
	//return csvCreator(argc, argv);

	/// FaceRecognizer.exe
	//return faceRecognizer(argc, argv);

	/// trainAndSaveModel.exe
	//return trainAndSaveModel(argc, argv);

	/// topN.exe
	return topN(argc, argv);

	//return exportDir(argc, argv);

	//return maskDir(argc, argv);

	//return 0;
}