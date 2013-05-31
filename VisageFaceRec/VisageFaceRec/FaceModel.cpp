#include "FaceModel.h"

FaceModel::FaceModel(){
}

FaceModel::FaceModel(int modelType, vector<Person> people)
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

	loadImagesFromPeople(people);

	model->train(this->trainnedImages, this->trainnedLabels);
}

FaceModel::FaceModel(int modelType, vector<Person> people, double threshold, int nComponents)
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

	loadImagesFromPeople(people);

	model->train(this->trainnedImages, this->trainnedLabels);

}

FaceModel::FaceModel(int modelType, const string& filename){
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

	load(filename);
};

string FaceModel::getName()
{
	return this->name;
}

int FaceModel::getSizeTrainnedImages(){
	return this->trainnedImages.size();
}

void FaceModel::save(const string& filename) const{
	this->model->save("..//saved//" + filename);
};


void FaceModel::load(const string& filename){
	this->model->load("..//saved//" + filename);
};

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