#include "algorithmsTest.h"

void testModel(Ptr<FaceRecognizer>& model, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labelsTrain, vector<int>& labelsTest, vector<string>& names, ofstream& outputfile){
	clock_t tStart, tEnd;
	double timespent;
    
	outputfile << "Trainning " << imagesTrain.size() << " images." << endl;
	outputfile << endl;
	tStart = clock();
	model->train(imagesTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained "<<  model->name() <<" in " << timespent << " seconds" << endl;
	
	tStart = clock();
	unsigned int nImagesToTest = imagesTest.size();
	outputfile << endl;
	outputfile << "Testing " << nImagesToTest << " different images." << endl;

	outputfile << endl;
	outputfile << setw(35) << "Class " << setw(8) << "Pred" << setw(15) << "Conf" << endl;
	
	int rightPredictions = 0;
	for(unsigned int i=0; i<nImagesToTest; i++){
		Mat testSample = imagesTest[i];
		int testLabel = labelsTest[i];
		
		double predictedConfindence = 0.0;
		int predictedLabel = -1;

		model->predict(testSample, predictedLabel, predictedConfindence);


		stringstream labeledName;
		labeledName << names[i] << "  " << testLabel;
		//labeledName << testLabel;
		outputfile << setw(35) << labeledName.str();

		if(predictedLabel != testLabel){
			outputfile << setw(8) << predictedLabel << setw(15) << predictedConfindence;
		}
		else{
			outputfile << setw(8) << "-" << setw(15) << predictedConfindence;
			rightPredictions++;
		}

		outputfile << endl;
	}
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Tested in " << timespent << " seconds" << endl;

	outputfile << endl;
	double percentage = (double)rightPredictions/(double)nImagesToTest * 100.0;

	outputfile << "Right predictions: " << rightPredictions << ", " << percentage << "%" << endl;
}

void testAll(vector<Mat> imagesTrain, vector<Mat> imagesTest, vector<int> labelsTrain, vector<int> labelsTest, ofstream outputfile){
	clock_t tStart, tEnd;
	double timespent;

	Ptr<FaceRecognizer> modelEigen = createEigenFaceRecognizer();
	Ptr<FaceRecognizer> modelFisher = createFisherFaceRecognizer();
	Ptr<FaceRecognizer> modelLBPH = createLBPHFaceRecognizer();
    
	outputfile << "Trainning " << imagesTrain.size() << " images." << endl;
	outputfile << endl;
	tStart = clock();
	modelEigen->train(imagesTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained Eigenfaces model in " << timespent << " seconds" << endl;

	tStart = clock();
	modelFisher->train(imagesTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained Fisherfaces model in " << timespent << " seconds" << endl;

	tStart = clock();
	modelLBPH->train(imagesTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained LBPH model in " << timespent << " seconds" << endl;
	
	int rightPredictionsEigen = 0, rightPredictionFisher = 0, rightPredictionLBPH = 0;
	tStart = clock();
	unsigned int nImagesToTest = imagesTest.size();
	outputfile << endl;
	outputfile << "Testing " << nImagesToTest << " different images." << endl;

	outputfile << endl;
	outputfile << setw(25) << "Class" << setw(8) << "Eigen" << setw(15) << "Conf" << setw(8) <<"Fisher" << setw(15) << 
		"Conf" << setw(8) << "LBPH" << setw(15) << "Conf" << endl;

	for(unsigned int i=0; i<nImagesToTest; i++){
		Mat testSample = imagesTest[i];
		int testLabel = labelsTest[i];
		
		double predictedConfindenceEigen = 0.0, predictedConfindenceFisher = 0.0, predictedConfindenceLBPH = 0.0;
		int predictedLabelEingen = -1, predictedLabelFisher = -1, predictedLabeLBPH = -1;

		modelEigen->predict(testSample, predictedLabelEingen, predictedConfindenceEigen);
		modelFisher->predict(testSample, predictedLabelFisher, predictedConfindenceFisher);
		modelLBPH->predict(testSample, predictedLabeLBPH, predictedConfindenceLBPH);


		stringstream labeledName;
		//labeledName << names[i] << "-" << testLabel;
		labeledName << testLabel;
		outputfile << setw(25) << labeledName.str();
		if(predictedLabelEingen != testLabel){
			outputfile << setw(8) << predictedLabelEingen << setw(15) << predictedConfindenceEigen;
		}
		else{
			outputfile << setw(8) << "-" << setw(15) << predictedConfindenceEigen;
			rightPredictionsEigen++;
		}

		if(predictedLabelFisher != testLabel){
			outputfile << setw(8) << predictedLabelFisher << setw(15) << predictedConfindenceFisher;
		}
		else{
			outputfile << setw(8) << "-" << setw(15) << predictedConfindenceFisher;
			rightPredictionFisher++;
		}

		if(predictedLabeLBPH != testLabel){
			outputfile << setw(8) << predictedLabeLBPH << setw(15) << predictedConfindenceLBPH;
		}
		else{
			outputfile << setw(8) << "-" << setw(15) << predictedConfindenceLBPH;
			rightPredictionLBPH++;
		}

		outputfile << endl;
	}
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Tested in " << timespent << " seconds" << endl;

	outputfile << endl;
	double percentageEigen = (double)rightPredictionsEigen/(double)nImagesToTest * 100.0;
	double percentageFisher = (double)rightPredictionFisher/(double)nImagesToTest * 100.0;
	double percentageLBPH = (double)rightPredictionLBPH/(double)nImagesToTest * 100.0;

	outputfile << "Right predictions EigenFaces: " << rightPredictionsEigen << ", " << percentageEigen << "%" << endl;
	outputfile << "Right predictions FisherFaces: " << rightPredictionFisher << ", " << percentageFisher << "%" << endl;
	outputfile << "Right predictions LBPH: " << rightPredictionLBPH << ", " << percentageLBPH << "%" << endl;

}

void algorithmsTest(const string path, const string outputfilename) {
	ofstream outputfile;
	outputfile.open(outputfilename);

    // DATA
	vector<Mat> imagesTrain;
	vector<Mat> imagesTest;
	vector<int> labelsTrain;
	vector<int> labelsTest;
	vector<string> names;
	string imagesPath;

	clock_t tStart, tEnd;
	double timespent;
	
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        //read_csv_lfw(path, names, imagesTrain, imagesTest, labelsBW);
		tStart = clock();
		imagesPath = read_csv(path, 8, imagesTrain, imagesTest, labelsTrain, labelsTest, names);
		tEnd = clock();
		timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);

		int totalImages = imagesTrain.size() + imagesTest.size();

		outputfile << "Source file: " << path << endl;
		outputfile << "Images path: " << imagesPath << endl << endl;
		outputfile << "Loaded " << totalImages << " images in " << timespent << " seconds" << endl;
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << path << "\". Reason: " << e.msg << endl;
        // ERROR
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(imagesTrain.size() <= 1) {
        string error_message = "This program needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

	Ptr<FaceRecognizer> modelEigen = createEigenFaceRecognizer();
	testModel(modelEigen, imagesTrain, imagesTest, labelsTrain, labelsTest, names, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

	Ptr<FaceRecognizer> modelFisher = createFisherFaceRecognizer();
	testModel(modelFisher, imagesTrain, imagesTest, labelsTrain, labelsTest, names, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

	Ptr<FaceRecognizer> modelLBPH = createLBPHFaceRecognizer();
	testModel(modelLBPH, imagesTrain, imagesTest, labelsTrain, labelsTest, names, outputfile);
	outputfile << endl << "-------------------------------------------------------------------------------" << endl;

	cout << "Results in " << outputfilename << endl;
	outputfile.close();

    //waitKey(0);
}