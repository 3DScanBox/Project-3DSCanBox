// 3DScanBox.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

// GUIDELINES FOR WRITING CODE:
// 1.) use meaningful names for variables, functions etc.
// 2.) when introducing new variable (that is used often/is important) explain with a short comment what it's used for
// 3.) define functions for sub-tasks, to keep code as short and clear as possible
// 4.) when writing new code/functions comment: "// written by ... on x.x.18"
// 5.) when editing code from someone else comment: "// edited by ... on x.x.18"
// 6.) comment-out not-working code (use "/* */" to comment big blocks of code) , program should always be in a working state. 

#pragma region INCLUDES

#include "stdafx.h"				// needed for visual studio
#include <vector>				// vector-construct
#include <string>
#include <sstream>				// string manipulations
#include <fstream>				// used to read text-file with camera parameters
// basic opencv functionalities
#include <opencv2/opencv.hpp>	
#include "opencv2/highgui.hpp"	// reading and writing images
#include <opencv2/core/core.hpp>// displaying images with imshow

#pragma endregion INCLUDES

using namespace std; // don't need to write std::
using namespace cv;

#pragma region FUNCTION-DEFINITIONS
void getSourceImages(vector<Mat> &sourceImages);
void getCameraMatrix(vector<Mat> &H, vector<Mat> &R, vector<Mat> &t);
#pragma endregion FUNCTION-DEFINITIONS

#pragma region GLOBAL-VARIABLES
// use global variables considerate, put "normal" variables in the main() function
// array that holds strings where we load the images from
string sourcePaths[1] = {
	"../../DataSets/MultiViewImages/templeDataSet/templeSparseRing/templeSparseRing/templeSR"
};
// number indicating which sourcePath we actually want to select
int nrOfPath = 0;
#pragma endregion GLOBAL-VARIABLES



int main()
{
#pragma region IMPORTANT VARIABLE DECLARATION
	// source images (color images that we take) are saved in this vector
	vector<Mat> sourceImages;
	// here the camera orientations w.r.t. the Initial system is saved (R)
	vector<Mat> cameraOrientations;
	// here the camera translations w.r.t. the initial system is saved (t)
	vector<Mat> cameraTranslations;
	// here the camera matrices are saved. Camera matrix = K * [R|t] is a 3x4 Matrix.
	vector<Mat> cameraMatrices;
#pragma endregion

#pragma region PART 1
	// PART 1: CALIBRATION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// output: EXTRINSIC & INTRINSIC CALIBRATION FILES
	// PART 1a) if extrinsic & intrinsic files exist ask user if he wants to perfom camera calibration, if they dont exist, do camera calibration
	
	// PART 1b) READ CHESS-BOARD IMAGES 
	// output: opencv image objects

	// PART 1c) GET ORIENTATION-MATRIX R AND PRINCIPAL POINT C for every CAMERA
	// output:
	// Ri = [r1i, r2i, r3i] = orientation matrix of camera i
	// Ci = r_ci = position vector expressed in Inertial frame of camera i


	// PART 1 FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion PART 1

#pragma region PART 2
	// PART 2: DEPTH MAPS CONSTRUCTION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// output: death map for each Camera

	// PART 2a) read images and camera parameters from files.
	// written by Luc on 26.3:
	getSourceImages(sourceImages);
	getCameraMatrix(cameraMatrices, cameraOrientations, cameraTranslations);
#pragma region testing
	/*
	// check if values are correct:
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			cout << cameraMatrices[0].at<double>(i, j) << ", ";
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			cout << cameraMatrices[15].at<double>(i, j) << ", ";
		cout << endl;
	}
	*/
#pragma endregion 

	// PART 2b) perform pair-wise rectification.
#pragma region testing
	Mat Rrelative = cameraOrientations[1].t() * cameraOrientations[0];
	Mat Trelative = cameraTranslations[1] - cameraTranslations[0];
#pragma endregion
	// PART 2c) get disparity images
	
	// PART 2d) compute depth images


	//PART 2 FINISHED%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion

#pragma region PART 3
	// PART 3: FUSION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// output: point-cloud, NEEDS TO BE SPECIFIED MORE

	// PART 3 FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion 

#pragma region PART 4
	// PART 4: MESHING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// PART 4 FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion  

#pragma region PART 5
	// PART 5: HOLE CLOSING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// PART 5: HOLE CLOSING FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion 


    return 0;
}

// written by Luc, last edited on 26.03
// reads source Images as RGB image and saves them into sourceImages
void getSourceImages(vector<Mat> &sourceImages) {
	// read first line of text-file which specifies the number of images in the data-set
	ifstream paramFile(sourcePaths[nrOfPath] + "_par.txt");
	string textLine;
	getline(paramFile, textLine);
	int nrOfImages = stoi(textLine);
	// Read test-images and save them into a vector:
	for (int i = 1; i <= nrOfImages; i++) {
		char number[5];
		snprintf(number, 5, "%04d", i);
		sourceImages.push_back(imread(sourcePaths[nrOfPath] + number + ".png", IMREAD_COLOR));
	}
}

// written by Luc, last edited on 26.3
// reads cameraParameters and calculates the cameraMatrix (=K * [R|t]), Orientation and Translation w.r.t. the inertial frame for each camera)
void getCameraMatrix(vector<Mat> &H, vector<Mat> &R, vector<Mat> &t) {
	ifstream paramFile(sourcePaths[nrOfPath] + "_par.txt");
	string textLine;
	getline(paramFile, textLine);
	while (getline(paramFile, textLine)) {
		// create matrices and fill them with 0
		Mat camMatrix = Mat::zeros(3, 4, CV_64F), K = Mat::zeros(3, 3, CV_64F), Rt = Mat::zeros(3, 4, CV_64F);

		// read all parameters from textLine
		istringstream buf(textLine);
		string word;
		for (int i = 0; buf >> word; ++i) {
			if (i == 0) {
				continue;
			}
			else if (i < 10) {
				int j = i - 1;
				K.at<double>(j / 3, j % 3) = stod(word);
			}
			else if (i < 19) {
				int j = i - 1 - 9;
				Rt.at<double>(j / 3, j % 3) = stod(word);
			}
			else if (i <= 21) {
				int j = i - 1 - 9 - 9;
				Rt.at<double>(j, 3) = stod(word);
			}
			else cout << "Error when parsing cameraParameters" << endl;
		}
		Mat Ri = Rt(Rect(0, 0, 3, 3));
		Mat t = Rt(Rect(3, 0, 1, 3));
		camMatrix = K * Rt;
		R.push_back(Ri);
		t.push_back(t);
		H.push_back(camMatrix);
	}
}
