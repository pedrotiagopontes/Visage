#include "utils.h"

static string nomalize_number(string num){
	int i = atoi(num.c_str());
	string newNum ="";
	if(i < 1000){
		newNum += "0";
		if(i < 100){
			newNum += "0";
			if(i < 10)
				newNum += "0"; 
		}
	}
	return newNum + num;
}

void read_and_createDir(const string& filename, const string& outputname, vector<Mat>& images, vector<string>& dirs, vector<string>& names, char separator) {
    ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
	
	string path;
	getline(file, path);

    string line, name, pic;
	int i=0;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, name, '\\');
		getline(liness, pic, separator);
        if(!path.empty()) {
			string dirname ="mkdir "+ outputname +"\\"+ name;
			system(dirname.c_str());
			images.push_back(imread(path+name+"\\"+pic, CV_LOAD_IMAGE_COLOR));
			dirs.push_back(name);
			names.push_back(pic);
        }
    }
}

string read_csv(const string& filename, int trainedImgsPerClass, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labelsTrain, vector<int>& labelsTest, vector<string>& names, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
	
	string path;
	getline(file, path);

    string line, name, classlabel, lastLabel="-1";
	int i=0;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, name, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
			if(classlabel != lastLabel){
				i=0;
				lastLabel = classlabel;
			}
			if(i<trainedImgsPerClass){
				Mat im1 = imread(path+name, CV_LOAD_IMAGE_GRAYSCALE);
				if(im1.rows > 0){
					//cout << path << name << " -"<<i<<"- " << im1.rows <<"x"<< im1.cols<<endl;
					imagesTrain.push_back(im1);
					labelsTrain.push_back(atoi(classlabel.c_str()));
					//cout << name << ";"<<classlabel << endl;
				}else{
					cout << "ERROR loading " << path+name << endl;
				}
			}else{
				Mat im2 = imread(path+name, CV_LOAD_IMAGE_GRAYSCALE);
				if(im2.rows > 0){
					//cout << path << name << " -test"<<i<<"- " << im2.rows <<"x"<< im2.cols<<endl;
					imagesTest.push_back(im2);
					names.push_back(name);
					labelsTest.push_back(atoi(classlabel.c_str()));
				}else{
					cout << "ERROR loading " << path+name <<endl;
				}
			}
        }
		i++;
    }
	return path;
}

//outdated!
void read_csv_pairs(const string& filename, vector<string> &names, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labels, char separator, string ext) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
	int label = 0;
	//First line must be path
	string path;
	getline(file, path);

    string lastname, line, name, fig1, fig2;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, name, separator);
		if(lastname != name){
			label++;
		}
		lastname = name;
		getline(liness, fig1, separator);
		getline(liness, fig2);
		fig1 = nomalize_number(fig1);
		fig2 = nomalize_number(fig2);

		if(!name.empty() && !path.empty() && !fig1.empty() && !fig2.empty()) {
			string im1 = path+name+"_"+fig1+ext;
			string im2 = path+name+"_"+fig2+ext;
			//string im1 = path+name+"\\"+name+"_"+fig1+ext;
			//string im2 = path+name+"\\"+name+"_"+fig2+ext;
			Mat mat1 = imread(im1, CV_LOAD_IMAGE_GRAYSCALE);
			Mat mat2 = imread(im2, CV_LOAD_IMAGE_GRAYSCALE);
			imagesTrain.push_back(mat1);
			imagesTest.push_back(mat2);
			//cout << im1 << " - " << mat1.rows <<"x"<< mat1.cols<<endl;
			//cout << im2 << " - " << mat2.rows <<"x"<< mat2.cols<<endl;
			
            labels.push_back(label);
			names.push_back(name);
        }
    }
}

void readLfwStats(const string& filename, string outputfilename, vector<int>& labelsPerClass, vector<string>& names){
	ifstream file(filename.c_str(), ifstream::in);
	ofstream outputfile;
	outputfile.open("..\\results\\"+outputfilename);

    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
	string path;
	getline(file, path);
    string line, lastname, name, img, classlabel;
	string lastclasslabel = "-1";
	int count = 0;
	int max = 0;

    while (getline(file, line)) {

        stringstream liness(line);
        getline(liness, name, '\\');
        getline(liness, img, ';');
		getline(liness, classlabel);

        if(!path.empty() && !classlabel.empty() && !name.empty()) {
			if(lastclasslabel == classlabel){
				count++;
			}
			else{
				if(count > 0){
					labelsPerClass.push_back(count);
					names.push_back(lastname);
					outputfile << setw(30) << lastname << setw(5) << count << endl;
				}
				if(count > max){
					max = count;
				}
				count = 1;
			}
			lastclasslabel = classlabel;
			lastname = name;
        }
    }
	labelsPerClass.push_back(count);
	names.push_back(lastname);
	outputfile << setw(30) << lastname << setw(5) << count << endl;

	vector<int> stats;
	for(int i=0; i <= max; i++)
		stats.push_back(0);

	for(unsigned int i=0; i < labelsPerClass.size(); i++){
		stats[labelsPerClass[i]]++;
	}

	outputfile << endl;
	int sum=0;
	for(unsigned int i=0; i<stats.size(); i++){
		if(stats[i] > 0){
			outputfile << "Classes with " << i << " images: " << stats[i] << "| >= " << names.size() - sum<<  endl;
			sum+= stats[i];
		}
	}
	outputfile.close();
}

void createCSV(string filename, vector<int> imgsPerClass, vector<string> classes, int bottomLimit, int topLimit, string fileExtension){
	ofstream outputfile;
	outputfile.open("..\\etc\\" + filename);

	for(unsigned int i=0; i < imgsPerClass.size(); i++){
		if(imgsPerClass[i] >= bottomLimit){
			int picInt = 1;
			while(picInt <= imgsPerClass[i] && picInt <= topLimit){
				stringstream ss;
				ss << picInt;
				//outputfile << classes[i] << "\\" << classes[i] << "_"<<nomalize_number(ss.str()) << fileExtension <<";" <<i << endl;
				outputfile << classes[i] << "_"<<nomalize_number(ss.str()) << fileExtension <<";" <<i << endl;
				picInt++;
			}
		}
	}

}