#include "Person.h"

Person::Person(){
};

Person::Person(int label, string name, string imageDir)
{
	this->label = label;
	this->name = name;
	this->imageDir = imageDir;
}

Person::Person(int label, string name, string imageDir, vector<string> trainImages, vector<string> testImages)
{
	this->label = label;
	this->name = name;
	this->imageDir = imageDir;

	for(unsigned int i=0; i < trainImages.size(); i++){
		this->addTrainImage(trainImages[i]);
	}

	for(unsigned int i=0; i < testImages.size(); i++){
		this->addTestImage(testImages[i]);
	}
}

string Person::toString(){
	stringstream ss;
	ss << this->name << ": " << this->label;

	return ss.str();
}

string Person::toStringWithImages(){
	stringstream ss;
	ss << this->name << ": " << this->label << endl;

	for(unsigned int j=0; j < trainImages.size(); j++){
		ss << '\t' << trainImages[j] << endl;
	}

	for(unsigned int k=0; k < testImages.size(); k++){
		ss << '\t' << testImages[k]<< " test" << endl;
	}

	return ss.str();
}

int Person::getLabel(){
	return this->label;
}

string Person::getName(){
	return this->name;
}

string Person::getImageDir(){
	return this->imageDir;
}

vector<string> Person::getTrainImages(){
	return this->trainImages;
}

vector<string> Person::getTestImages(){
	return this->testImages;
}

void Person::addTrainImage(string image){
	this->trainImages.push_back(image);
}

void Person::addTestImage(string image){
	this->testImages.push_back(image);
}