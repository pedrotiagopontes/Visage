#include "algorithmsTest.h"

void algorithmsTest(const string path, const string outputfilename) {
	ofstream outputfile;
	outputfile.open(outputfilename);

    // DATA
	vector<Mat> imagesBWTrain;
	vector<Mat> imagesBWTest;
	vector<int> labelsTrain;
	vector<int> labelsTest;
	vector<string> names;
	clock_t tStart, tEnd;
	double timespent;
	
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        //read_csv_lfw(path, names, imagesBWTrain, imagesBWTest, labelsBW);
		tStart = clock();
			read_csv(path, 8, imagesBWTrain, imagesBWTest, labelsTrain, labelsTest);
		tEnd = clock();
		timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);

		int totalImages = imagesBWTest.size() + imagesBWTrain.size();

		outputfile << "Loaded " << totalImages << " images in " << timespent << " seconds" << endl;
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << path << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    // Quit if there are not enough images for this demo.
    if(imagesBWTrain.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original size:
    int height = imagesBWTrain[0].rows;

	Ptr<FaceRecognizer> modelEigen = createEigenFaceRecognizer(10);
	//Ptr<FaceRecognizer> modelFisher = createFisherFaceRecognizer();
	//Ptr<FaceRecognizer> modelLBPH = createLBPHFaceRecognizer();
    
	outputfile << "Trainning " << imagesBWTrain.size() << " images." << endl;
	outputfile << endl;
	tStart = clock();
	modelEigen->train(imagesBWTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained Eigenfaces model in " << timespent << " seconds" << endl;
/*
	tStart = clock();
	modelFisher->train(imagesBWTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained Fisherfaces model in " << timespent << " seconds" << endl;

	tStart = clock();
	modelLBPH->train(imagesBWTrain, labelsTrain);
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Trained LBPH model in " << timespent << " seconds" << endl;
*/	
	int rightPredictionsEigen = 0, rightPredictionFisher = 0, rightPredictionLBPH = 0;
	tStart = clock();
	unsigned int nImagesToTest = imagesBWTest.size();
	outputfile << endl;
	outputfile << "Testing " << nImagesToTest << " different images." << endl;

	outputfile << endl;
	outputfile << setw(25) << "Class" << setw(8) << "Eigen" << setw(15) << "Conf" << setw(8) <<"Fisher" << setw(15) << 
		"Conf" << setw(8) << "LBPH" << setw(15) << "Conf" << endl;

	//outputfile << setw(25) << "Class" << setw(8) << "Eigen" << setw(15) << "Conf" << setw(8) << "LBPH" << setw(15) << "Conf" << endl;
	for(unsigned int i=0; i<nImagesToTest; i++){
		Mat testSample = imagesBWTest[i];
		int testLabel = labelsTest[i];
		
		double predictedConfindenceEigen = 0.0, predictedConfindenceFisher = 0.0, predictedConfindenceLBPH = 0.0;
		int predictedLabelEingen = -1, predictedLabelFisher = -1, predictedLabeLBPH = -1;

		modelEigen->predict(testSample, predictedLabelEingen, predictedConfindenceEigen);
/*		modelFisher->predict(testSample, predictedLabelFisher, predictedConfindenceFisher);
		modelLBPH->predict(testSample, predictedLabeLBPH, predictedConfindenceLBPH);
*/

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
/*
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
*/
		outputfile << endl;
	}
	tEnd = clock();
	timespent = (double)((tEnd - tStart)/CLOCKS_PER_SEC);
	outputfile << "Tested in " << timespent << " seconds" << endl;

	outputfile << endl;
	double percentageEigen = (double)rightPredictionsEigen/(double)nImagesToTest * 100.0;
/*	double percentageFisher = (double)rightPredictionFisher/(double)nImagesToTest * 100.0;
	double percentageLBPH = (double)rightPredictionLBPH/(double)nImagesToTest * 100.0;
*/
	outputfile << "Right predictions EigenFaces: " << rightPredictionsEigen << ", " << percentageEigen << "%" << endl;
/*	outputfile << "Right predictions FisherFaces: " << rightPredictionFisher << ", " << percentageFisher << "%" << endl;
	outputfile << "Right predictions LBPH: " << rightPredictionLBPH << ", " << percentageLBPH << "%" << endl;
*/

	cout << "Results in " << outputfilename << endl;
	outputfile.close();


    //waitKey(0);
}