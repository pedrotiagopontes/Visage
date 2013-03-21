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

int FaceModel::testModelNPredictions(vector<Person> people, ofstream& outputfile, size_t n){

	vector<int> rightPredictions;
	rightPredictions.resize(n, 0);
	int nImages= 0;

	clock_t tStart = clock();
	outputfile << endl;
	outputfile << setw(35) << "Image" << setw(8) << "Label" << " | ";
	for(size_t j = 0; j < n; j++){
		outputfile << setw(2) << j+1 <<"º "<< setw(4) << "Pred" << setw(10) << "Conf "<< " | ";
	}
	outputfile << endl;

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
				outputfile << setw(35) << images[k] << setw(8) << people[i].getLabel() << " | ";

				vector<int> labels;
				vector<double> confs;
				identityImage(image, n, labels, confs);

				for(size_t j = 0; j < n && j < labels.size(); j++){
					if (people[i].getLabel() == labels[j]){
						outputfile << setw(8) << "-" << setw(10) << confs[j] << " | ";
						rightPredictions[j]++;
					}else{
						outputfile << setw(8) << labels[j] << setw(10) << confs[j] << " | ";
					}
				}
				outputfile << endl;
				nImages++;
				
			}else{
				cout << "ERROR loading test " << images[k] << endl;
				outputfile << endl;
			}
		}
	}
	outputfile << endl;

	outputfile << "Tested " << nImages <<"images in " << timespent(tStart) << " seconds" << endl;
	int totalRight = 0;
	double totalPercentage = 0;
	for(size_t j = 0; j < n; j++){
		double percentage = (double)rightPredictions[j]/(double)nImages * 100.0;
		totalRight += rightPredictions[j];
		totalPercentage += percentage;

		outputfile << "Right predictions in level "<< j+1 << ": " << rightPredictions[j] << ", " << percentage << "%  | totals: "<< totalRight << ", " << totalPercentage << "%"  << endl;
	}

	return rightPredictions.size();
}

string FaceModel::getName()
{
	return this->name;
}