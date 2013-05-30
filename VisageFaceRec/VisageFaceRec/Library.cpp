#include "Library.h"



Library::Library(string filename, double percentageTrainnedImages)
{
	this->filename = filename;
	this->percentageTrainnedImages = percentageTrainnedImages;
	this->minImagesPerson = INT_MAX;
	this->maxImagesPerson = 0;
	this->totalImages = 0;

	load_csv(filename);
}

Library::Library(){
}

Library::~Library(void)
{
}

void Library::addPerson(int classlabel, string name, string imageDir,  vector<string>& tempImages){
	double nImages = (double)tempImages.size() * (this->percentageTrainnedImages/100.0);

	Person p1(classlabel, name, imageDir);
	for(int i=0; i<(int)nImages; i++){
		p1.addTrainImage(tempImages[i]);
	}
	for(unsigned int i=(int)nImages; i<tempImages.size(); i++){
		p1.addTestImage(tempImages[i]);
	}

	this->minImagesPerson = MIN(tempImages.size(), this->minImagesPerson);
	this->maxImagesPerson = MAX(tempImages.size(), this->maxImagesPerson);

	people.push_back(p1);

	mapPeople.insert(pair<int,Person>(classlabel, p1));
}

Person Library::getPerson(int classlabel){
	return mapPeople[classlabel];
}

void Library::load_csv(const string filename, char separator){
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}

	//First Line of file must be dir path;
	getline(file, this->dirPath);

	vector<string> tempImages;
	string line, name, pic, classlabel, lastLabel="-1", lastName;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, name, '\\');
		getline(liness, pic, separator);
		getline(liness, classlabel);

		if(!dirPath.empty() && !classlabel.empty() && !name.empty()) {
			if(classlabel != lastLabel && tempImages.size() > 0){
				addPerson(atoi(lastLabel.c_str()), lastName, dirPath, tempImages);
				tempImages.clear();
			}
			//tempImages.push_back(dirPath+name+"\\"+pic);
			tempImages.push_back(pic);
			lastLabel = classlabel;
			lastName = name;
			this->totalImages++;
		}
	}

	addPerson(atoi(lastLabel.c_str()), lastName, dirPath, tempImages);
}

string Library::toString(){
	stringstream ss;

	ss << "Library: " << this->filename << endl;
	ss << "\tImages Dir: " << this->dirPath << endl;
	ss << "\tTotal Images: " << this->totalImages << endl;
	ss << "\tTotal People: "<< this->people.size() << endl;
	ss << "\tImages per person MIN | MAX: " << this->minImagesPerson << " | "<< this->maxImagesPerson << endl;
	ss << "\tPercentage of trainned images per person: " << this->percentageTrainnedImages << endl;

	return ss.str();
}

string Library::toStringWithImages(){
	stringstream ss;

	ss << toString();

	for(unsigned int i=0; i < this->people.size(); i++){
		ss << people[i].toStringWithImages() << endl;
	}
	return ss.str();
}

string Library::getDirPath(){
	return this->dirPath;
}