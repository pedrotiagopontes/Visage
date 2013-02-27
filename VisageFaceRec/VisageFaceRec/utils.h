#ifndef UTILS_H
#define UTILS_H
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace cv;
using namespace std;

static string nomalize_number(string num);
void read_csv_lfw(const string& filename, vector<string> &names, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labels, char separator = '\t', string ext = ".jpg");
void read_csv(const string& filename, int trainedImgsPerClass, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labelsTrain, vector<int>& labelsTest, vector<string>& names, char separator = ';');
void readLfwStats(const string& filename, string outputfilename, vector<int>& labelsPerClass, vector<string>& names);
void createCSV(string filename, vector<int> imgsPerClass, vector<string> classes, int bottomLimit, int topLimit, string fileExtension = ".jpg");
 
#endif