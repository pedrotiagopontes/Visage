#include "FaceModel.h"

FaceModel::FaceModel(int modelType, vector<Person> people, string storageFile)
{
	this->nComponents = -1;
	this->threshold = -1;

	switch (modelType)
	{
	case EIGENFACES:
		this->model = createEigenFaceRecognizerExtended();
		this->name = "EIGENFACES";
		break;

	case LBPH:
		this->model = createLBPHFaceRecognizerExtended();
		this->name = "LBPH";
		break;

	default:
		this->model = createFisherFaceRecognizerExtended();
		this->name = "FISHERFACES";
		break;
	}

	this->storageFile = storageFile;

	loadImagesFromPeople(people);

	model->train(this->trainnedImages, this->trainnedLabels);
}

FaceModel::FaceModel(int modelType, vector<Person> people, double threshold, int nComponents, string storageFile)
{
	this->nComponents = nComponents;
	this->threshold = threshold;

	switch (modelType)
	{
	case EIGENFACES:
		this->model = createFisherFaceRecognizerExtended(nComponents, threshold);
		this->name = "EIGENFACES";
		break;

	case LBPH:
		this->model = createLBPHFaceRecognizerExtended(); ///TO-DO review
		this->name = "LBPH";
		break;

	default:
		this->model = createFisherFaceRecognizerExtended(nComponents, threshold);
		this->name = "FISHERFACES";
		break;
	}

	this->storageFile = storageFile;

	loadImagesFromPeople(people);

	model->train(this->trainnedImages, this->trainnedLabels);

}

string FaceModel::getName()
{
	return this->name;
}

void FaceModel::loadImagesFromPeople(vector<Person> people)
{
	for(unsigned int i=0; i<people.size(); i++){
		vector<string> images = people[i].getTrainImages();
		string imageDir = people[i].getImageDir();
		string personName = people[i].getName();

		for(unsigned int k=0; k<images.size(); k++){
			Mat image = imread(imageDir+personName+"\\"+images[k], CV_LOAD_IMAGE_GRAYSCALE);
			if(image.rows > 0){
				this->trainnedImages.push_back(image);
				this->trainnedLabels.push_back(people[i].getLabel());
			}else{
				cout << "ERROR loading " << images[k] << endl;
			}
		}
	}
}

FaceModel::~FaceModel(void)
{
}

int FaceModel::identityImage(Mat image)
{
	return this->model->predict(image);
}

void FaceModel::identityImage(Mat image, int& label, double& confidence)
{
	this->model->predict(image, label, confidence);

}

void FaceModel::identityImage(Mat image, size_t n, vector<int> &labels, vector<double> &confidences)
{
	this->model->predictN(image, n, labels, confidences);
}

bool FaceModel::isSamePerson(int labelOriginal, Mat image, int& predictedLabel, double& confidence){
	identityImage(image, predictedLabel, confidence);

	return ((predictedLabel == labelOriginal) ? true : false);
}

int FaceModel::testModel(vector<Person> people, ofstream& outputfile){

	//vector<int> originalLabels;
	//vector<string> originalNames;
	//vector<int> resultsLabels;
	//vector<double> resultsConfidence;

	int rightPredictions = 0;
	int nImages= 0;

	clock_t tStart = clock();
	outputfile << endl;
	outputfile << setw(35) << "Image " << setw(8) << "Label" << setw(8) << "Pred" << setw(15) << "Conf" << endl;
	for(unsigned int i=0; i<people.size(); i++){
		vector<string> images = people[i].getTestImages();
		string imageDir = people[i].getImageDir();
		string personName = people[i].getName();

		outputfile << people[i].getName() << "  " << people[i].getTrainImages().size() << " | " << 
			people[i].getTestImages().size() << endl;

		for(unsigned int k=0; k<images.size(); k++){
			int label = -1;
			double confidence = -1.0;
			Mat image = imread(imageDir+personName+"\\"+images[k], CV_LOAD_IMAGE_GRAYSCALE);

			//ensure that image was loaded
			if(image.rows > 0){
				outputfile << setw(35) << images[k] << setw(8) << people[i].getLabel();

				if (isSamePerson(people[i].getLabel(), image, label, confidence)){
					outputfile << setw(8) << "-" << setw(15) << confidence << endl;
					rightPredictions++;
				}else{
					outputfile << setw(8) << label << setw(15) << confidence << endl;
				}
				//originalLabels.push_back(people[i].getLabel());
				//originalNames.push_back(people[i].getName());
				//resultsLabels.push_back(label);
				//resultsConfidence.push_back(confidence);
				nImages++;

			}else{
				cout << "ERROR loading test " << images[k] << endl;
				outputfile << endl;
			}
		}
	}
	outputfile << endl;

	outputfile << "Tested " << nImages <<" in " << timespent(tStart) << " seconds" << endl;

	double percentage = (double)rightPredictions/(double)nImages * 100.0;

	outputfile << "Right predictions: " << rightPredictions << ", " << percentage << "%" << endl;

	return rightPredictions;
}

int FaceModel::testModelNPredictions(vector<Person> people, ofstream& outputfile, ofstream& csvFile, size_t n){
	
	vector<int> rightPredictions;
	vector<double> avgPredictions;
	rightPredictions.resize(n, 0);
	avgPredictions.resize(n, 0.0);
	int nImages= 0;

	clock_t tStart = clock();
	outputfile << endl;
	outputfile << setw(35) << "Image" << setw(8) << "Label" << " | ";
	for(size_t j = 0; j < n; j++){
		outputfile << setw(2) << j+1 <<"� "<< setw(4) << "Pred" << setw(10) << "Conf "<< " | ";
	}
	outputfile << endl;

	for(unsigned int i=0; i<people.size(); i++){
		vector<string> images = people[i].getTestImages();
		string imageDir = people[i].getImageDir();
		string personName = people[i].getName();
		vector<int> rightPredictionsPerson;
		rightPredictionsPerson.resize(n, 0);
		int imagesTested = 0;

		outputfile << people[i].getName() << "  " << people[i].getTrainImages().size() << " | " << 
			people[i].getTestImages().size() << endl;

		for(unsigned int k=0; k<images.size(); k++){
			int label = -1;
			double confidence = -1.0;
			Mat image = imread(imageDir+personName+"\\"+images[k], CV_LOAD_IMAGE_GRAYSCALE);

			//ensure that image was loaded
			if(image.rows > 0){
				outputfile << setw(35) << images[k] << setw(8) << people[i].getLabel() << " | ";

				vector<int> labels;
				vector<double> confs;
				identityImage(image, n, labels, confs);

				for(size_t j = 0; j < n && j < labels.size(); j++){
					if (people[i].getLabel() == labels[j]){
						outputfile << setw(8) << "-" << setw(10) << confs[j] << " | ";
						rightPredictions[j]++;
						rightPredictionsPerson[j]++;
					}else{
						outputfile << setw(8) << labels[j] << setw(10) << confs[j] << " | ";
					}
				}
				outputfile << endl;
				nImages++;
				imagesTested++;

			}else{
				cout << "ERROR loading test " << images[k] << endl;
				outputfile << endl;
			}//end of image testing
		}//end of person images testing

		//OUTPUT PERSON AVG
		int totalRight = 0;
		double totalPercentage = 0;
		outputfile << setw(45) << "Total [level : accumu..] |";
		outputfile.precision(1);
		for(size_t j = 0; j < n; j++){
			double percentage = (double)rightPredictionsPerson[j]/(double)imagesTested * 100.0;
			totalRight += rightPredictionsPerson[j];
			totalPercentage += percentage;

			avgPredictions[j]+=percentage;
			if(rightPredictionsPerson[j] > 0){
				outputfile <<setw(5) << fixed << percentage <<"%:"<<setw(2)<<rightPredictionsPerson[j];
			}else{
				outputfile <<setw(9) << " ";
			}
			outputfile <<" |" <<setw(5) << fixed << totalPercentage << "%:"<<setw(2) << totalRight << "|";

		}
		outputfile << endl;

	}//end of all the people testing
	outputfile << endl;

	outputfile << setw(35) << " " << setw(8) << " " << " | ";
	for(size_t j = 0; j < n; j++){
		outputfile << setw(2) << j+1 <<"� "<< setw(4) << "Level" << setw(9) << " "<< " | ";
	}
	outputfile << endl;

	double totalPercentage = 0;
	outputfile <<string(21*n+46, '=') << endl;
	stringstream ss;
	ss <<  " RECOGNITION % AVG per Person("<<people.size()<<" people) | ";
	outputfile << setw(46) <<ss.str();
	totalPercentage = 0;
	outputfile.precision(2);
	vector<double> percentages;
	for(size_t j = 0; j < n; j++){
		double percentage = (double)avgPredictions[j]/(double)people.size();
		percentages.push_back(percentage);
		csvFile << percentage << ";";
	}
	csvFile << endl;

	for(size_t j = 0; j < n; j++){
		totalPercentage += percentages[j];
		outputfile <<setw(6) << fixed << percentages[j] <<"%   ";
		outputfile <<":" <<setw(6) << fixed << totalPercentage << "% | ";
		csvFile << totalPercentage << ";";
	}

	outputfile << endl;
	csvFile << endl;
	outputfile <<string(21*n+46, '=') << endl;

	outputfile.precision(0);
	ss.str("");
	ss << " RECOGNITION % FLAT ("<<nImages<<" images) | ";
	outputfile << setw(46) << ss.str();
	int totalRight = 0;
	totalPercentage = 0;
	vector<double> percentagesFlat;
	for(size_t j = 0; j < n; j++){
		double percentage = (double)rightPredictions[j]/(double)nImages * 100.0;
		percentagesFlat.push_back(percentage);
		csvFile << "=" << rightPredictions[j] <<"/" << nImages << ";";
	}
	csvFile << endl;

	for(size_t j = 0; j < n; j++){		
		totalRight += rightPredictions[j];
		totalPercentage += percentagesFlat[j];
		outputfile <<setw(3) << percentagesFlat[j] <<"% ("<<setw(2)<<rightPredictions[j] <<")";
		outputfile <<":" <<setw(4) << totalPercentage << "%("<<setw(2) << totalRight << ")| ";
		csvFile << "=" << totalRight <<"/" << nImages << ";";
	}
	outputfile << endl;
	csvFile << endl;
	outputfile <<string(21*n+46, '=') << endl;

	outputfile.precision(4);
	outputfile << "Tested " << nImages <<" images in " << timespent(tStart) << " seconds" << endl;

	return rightPredictions.size();
}


void FaceModel::testModelPrecision(vector<Person> people, ofstream& outputfile, ofstream& csvFile, size_t threshold){

	vector<int> rightPredictions;
	double avgPredictions = 0.0;
	double avgPredictionsFlat = 0.0;
	double avgTimesFlat = 0.0;
	double avgTimes = 0.0;
	rightPredictions.resize(threshold, 0);
	int nImages= 0;

	clock_t tStart = clock();
	outputfile << endl;
	outputfile << setw(35) << "Image" << setw(8) << "Label" << " | ";
	outputfile << setw(10) << "Precision"<< " | " << setw(5) << "Level"<< " | " << setw(10) << "Time(s)" << " | ";
	outputfile << endl;

	for(unsigned int i=0; i<people.size(); i++){
		vector<string> images = people[i].getTestImages();
		string imageDir = people[i].getImageDir();
		string personName = people[i].getName();
		double precisionRightPredictions = 0.0;
		double times = 0.0;
		int imagesTested = 0;

		outputfile << people[i].getName() << "  " << people[i].getTrainImages().size() << " | " << 
			people[i].getTestImages().size() << endl;

		for(unsigned int k=0; k<images.size(); k++){
			clock_t imageStartTime = clock();
			int label = -1;
			double confidence = -1.0;
			Mat image = imread(imageDir+personName+"\\"+images[k], CV_LOAD_IMAGE_GRAYSCALE);

			//ensure that image was loaded
			if(image.rows > 0){
				outputfile << setw(35) << images[k] << setw(8) << people[i].getLabel() << " | ";

				vector<int> labels;
				vector<double> confs;
				bool foundMatch = false;
				identityImage(image, threshold, labels, confs);
				for(size_t j = 0, counter= 1; j < threshold && j < labels.size(); j++, counter++){
					if (people[i].getLabel() == labels[j]){
						double precision =(1/(double)counter) * 100;
						double timeImage = timespent(imageStartTime);
						outputfile << setw(9) <<precision<< "% | " << setw(5) << counter<<  " | "<< setw(10) << timeImage << " | ";
						rightPredictions[j]++;
						precisionRightPredictions+=precision;
						times+=timeImage;
						avgPredictionsFlat+=precision;
						avgTimesFlat+=timeImage;
						foundMatch = true;
						break;
					}
				}

				if(!foundMatch){
					outputfile << setw(9) <<0<< "% | " << setw(5) << "INF"<<  " | "<< setw(10) << "INF" << " | ";
				}
				outputfile << endl;
				nImages++;
				imagesTested++;

			}else{
				cout << "ERROR loading test " << images[k] << endl;
				outputfile << endl;
			}//end of image testing
		}//end of person images testing
		outputfile << setw(45) << "AVG per Person |";
		precisionRightPredictions /= imagesTested;
		avgPredictions += precisionRightPredictions;
		times /= imagesTested;
		avgTimes += times;
		outputfile << setw(10) << precisionRightPredictions << "% | "<<setw(18)<< times << " | " << endl;

	}//end of all the people testing
	outputfile << endl;
	outputfile.precision(4);

	double totalPercentage = 0;
	outputfile <<string(80, '=') << endl;
	stringstream ss;
	ss <<  " RETRIEVAL AVG per Person("<<people.size()<<" people) | ";
	outputfile << setw(46) <<ss.str();
	totalPercentage = 0;
	avgPredictions /= people.size();
	avgTimes /= people.size();
	outputfile << setw(9) << avgPredictions << "% | " << setw(5) << 1/(avgPredictions/100)<<  " | "<< setw(10) << avgTimes << " | " << endl;
	csvFile << avgPredictions << ";" << 1/(avgPredictions/100) << ";" <<  avgTimes << endl;

	totalPercentage = 0;
	outputfile <<string(80, '=') << endl;
	ss.str("");
	ss <<  " RETRIEVAL AVG FLAT("<<nImages<<" images) | ";
	outputfile << setw(46) <<ss.str();
	totalPercentage = 0;
	avgPredictionsFlat /= nImages;
	avgTimesFlat /= nImages;
	outputfile << setw(9) << avgPredictionsFlat << "% | " << setw(5) << 1/(avgPredictionsFlat/100)<<  " | "<< setw(10) << avgTimesFlat << " | " << endl;
	csvFile << avgPredictionsFlat << ";" << 1/(avgPredictionsFlat/100) << ";" <<  avgTimesFlat << endl;
	outputfile <<string(80, '=') << endl;

	/*
	ss.str("");
	ss << " TOTAL (FLAT-"<<nImages<<" images) | ";
	outputfile << setw(46) << ss.str() << "level%  levelUnit : levelAccu% levelAccu|" << endl;
	int totalRight = 0;
	totalPercentage = 0;
	outputfile.precision(3);
	for(size_t j = 0; j < threshold; j++){
		double percentage = (double)rightPredictions[j]/(double)nImages * 100.0;
		totalRight += rightPredictions[j];
		totalPercentage += percentage;
		outputfile << setw(42) <<"level " <<j+1 << " | " << setw(6) << percentage <<"% "<<setw(9)<<rightPredictions[j] <<" ";
		outputfile <<": " <<setw(8) << totalPercentage << "% "<<setw(9) << totalRight << " | " << endl;
	}
	outputfile <<string(87, '=') << endl;
	*/
	outputfile.precision(4);
	outputfile << "Tested " << nImages <<" images in " << timespent(tStart) << " seconds" << endl;

}