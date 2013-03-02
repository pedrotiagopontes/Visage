#include "Library.h"

Library::Library(string filename, double percentageTrainnedImages)
{
	this->filename = filename;
	this->percentageTrainnedImages = percentageTrainnedImages;
	load_csv(filename);
}

Library::~Library(void)
{
}

void Library::addPerson(int classlabel, string name, vector<string>& tempImages){
	double nImages = (double)tempImages.size() * (this->percentageTrainnedImages/100.0);

	Person p1(classlabel, name);
	for(int i=0; i<nImages; i++){
		p1.addTrainImage(tempImages[i]);
	}
	for(int i=nImages; i<tempImages.size(); i++){
		p1.addTestImage(tempImages[i]);
	}

	people.push_back(p1);

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
				addPerson(atoi(lastLabel.c_str()), lastName, tempImages);
				tempImages.clear();
			}
			tempImages.push_back(dirPath+name+"\\"+pic);
			lastLabel = classlabel;
			lastName = name;
		}
    }
}