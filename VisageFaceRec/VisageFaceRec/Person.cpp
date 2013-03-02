#include "Person.h"


Person::Person(int label, string name)
{
	this->label = label;
	this->name = name;
}

Person::Person(int label, string name, vector<string> trainImages, vector<string> testImages)
{
	this->label = label;
	this->name = name;
	
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

int Person::getLabel(){
	return this->label;
}

string Person::getName(){
	return this->name;
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