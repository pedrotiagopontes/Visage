#ifndef ALGORITHMSTEST_H
#define ALGORITHMSTEST_H

#include "utils.h"

void algorithmsTest(const string path, const string outputfilename);
void testAll(vector<Mat> imagesTrain, vector<Mat> imagesTest, vector<int> labelsTrain, vector<int> labelsTest, ofstream outputfile);
void testModel(Ptr<FaceRecognizer>& model, vector<Mat>& imagesTrain, vector<Mat>& imagesTest, vector<int>& labelsTrain, vector<int>& labelsTest, vector<string>& names, ofstream& outputfile);

#endif