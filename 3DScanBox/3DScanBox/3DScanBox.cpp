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
using namespace cv;	 // don't need to write cv::

#pragma region FUNCTION-DEFINITIONS
void getSourceImages(vector<Mat> &sourceImages);
void getCameraMatrix(vector<Mat> &K, vector<Mat> &R, vector<Mat> &t);
#pragma endregion FUNCTION-DEFINITIONS

#pragma region GLOBAL-VARIABLES
// use global variables considerate, put "normal" variables in the main() function
// array that holds strings where we load the images from
string sourcePaths[3] = {
	"../../DataSets/MultiViewImages/templeDataSet/templeSparseRing/templeSparseRing/templeSR",
	"../../testing/testSquare.png",
	"../../DataSets/DisparityMaps/Adirondackperfect/"
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
	// here the camera matrices are saved. Camera matrix = K (3x3) Matrix.
	vector<Mat> cameraIntrinsic;
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
	// testing with testSquare:
	//sourceImages.push_back(imread(sourcePaths[1], IMREAD_COLOR));
	//sourceImages.push_back(imread(sourcePaths[1], IMREAD_COLOR));
	getCameraMatrix(cameraIntrinsic, cameraOrientations, cameraTranslations);

	/*
	//check results:
	cout << "K1 (default) = " << endl << cameraIntrinsic[0] << endl << endl;
	cout << "R1 (default) = " << endl << cameraOrientations[0] << endl << endl;
	cout << "t1 = " << endl << cameraTranslations[0] << endl << endl;
	cout << "K2 (default) = " << endl << cameraIntrinsic[1] << endl << endl;
	cout << "R2 (default) = " << endl << cameraOrientations[1] << endl << endl;
	cout << "t2 = " << endl << cameraTranslations[1] << endl << endl;
	*/
	/*
#pragma region PART 2B
	// PART 2b) perform pair-wise rectification.

	int i = 0;
	// try to rectivy first 2 images:
	vector<int> distCoefl = { 0, 0, 0, 0, 0 };
	vector<int> distCoefr = { 0, 0, 0, 0, 0 };
	
	// this is the case if their description is right:
	// lets try expresisng everything in the right frame:
	
	Mat Rrel = (cameraOrientations[0] * cameraOrientations[1].t());
	Mat Trel = -Rrel.t() * cameraTranslations[0] + cameraTranslations[1];
	
	// output-arguments:
	Mat Rl = Mat::zeros(3, 3, CV_64F);
	Mat Rr = Mat::zeros(3, 3, CV_64F);
	Mat Pl = Mat::zeros(3, 4, CV_64F);
	Mat Pr = Mat::zeros(3, 4, CV_64F);
	Mat Q = Mat::zeros(4, 4, CV_64F);
	Rect validPixl(0,0,250,250);
	Rect validPixr(0,0,250,250);
	// function-call
	stereoRectify(cameraIntrinsic[i], distCoefl, cameraIntrinsic[i+1], distCoefr, sourceImages[i].size(), Rrel, Trel, Rl, Rr, Pl, Pr, Q , CALIB_ZERO_DISPARITY, 1, sourceImages[i].size(), &validPixl, &validPixr);

	// lets check the output
	cout << "R1 (default) = " << endl << Rl << endl << endl;
	cout << "R2 (default) = " << endl << Rr << endl << endl;
	cout << "K1Unrectified = " << endl << cameraIntrinsic[i] << endl << endl;
	cout << "P1 (default) = " << endl << Pl << endl << endl;
	cout << "K2Unrectified = " << endl << cameraIntrinsic[i+1] << endl << endl;
	cout << "P2 (default) = " << endl << Pr << endl << endl;
	cout << "Q (default) = " << endl << Q << endl << endl;

	

	// find rectification map of first camera
	Mat Krectl = Mat::zeros(3, 3, CV_64F);
	for (int k = 0; k < 3; k++)
		for (int j = 0; j < 3; j++) { 
			double element = Pl.at<double>(k, j);
			Krectl.at<double>(k, j) = element;
		}
	//Mat Krect1New = getOptimalNewCameraMatrix(Krect1, distCoef1, sourceImages[0].size(),1, sourceImages[0].size(),0, true);
	// Mat Krect1New = getDefaultNewCameraMatrix(Krect1, sourceImages[0].size(), true);
	Mat maplx = Mat::zeros(sourceImages[i].rows, sourceImages[i].cols, CV_32F),
		maply = Mat::zeros(sourceImages[i].rows, sourceImages[i].cols, CV_32F);
	// this function fills map1x & map1y. Map1 maps the corresponding pixel position in the rectified image to the sourceImage.
	initUndistortRectifyMap(cameraIntrinsic[i], distCoefl, Rl, Pl, sourceImages[i].size(), CV_32FC1, maplx, maply );
	// test: try to get the map the other way around:
	// initUndistortRectifyMap(Krect1, distCoef1, R1.t(), cameraIntrinsic[0], sourceImages[0].size(), CV_32FC1, map1x, map1y);
	
	// same for second image
	Mat Krectr = Mat::zeros(3, 3, CV_64F);
	for (int k = 0; k < 3; k++)
		for (int j = 0; j < 3; j++) {
			double element = Pr.at<double>(k, j);
			Krectr.at<double>(k, j) = element;
		}
	//Mat Krect2New = getOptimalNewCameraMatrix(Krect2, distCoef2, sourceImages[1].size(), 1, sourceImages[1].size(), 0, true);
	//Mat Krect2New = getDefaultNewCameraMatrix(Krect2, sourceImages[1].size(), true);
	//cout << "Krect1 (default) = " << endl << Krect1New << endl << endl;
	//cout << "Krect2 (default) = " << endl << Krect2New << endl << endl;

	Mat maprx = Mat::zeros(sourceImages[i+1].rows, sourceImages[i+1].cols, CV_32F),
		mapry = Mat::zeros(sourceImages[i+1].rows, sourceImages[i+1].cols, CV_32F);
	// this function fills map2x & map2y. Map2 maps the corresponding pixel position in the rectified image to the sourceImage.
	initUndistortRectifyMap(cameraIntrinsic[i+1], distCoefr, Rr, Pr, sourceImages[i+1].size(), CV_32FC1, maprx, mapry);
	// test: try to get the map the other way around:
	// initUndistortRectifyMap(Krect2, distCoef2, R2.t(), cameraIntrinsic[1], sourceImages[1].size(), CV_32FC1, map2x, map2y);


	// try the remap function
	Mat rectImagel = Mat::zeros(sourceImages[i].rows, sourceImages[i].cols, IMREAD_COLOR);
	Mat rectImager = Mat::zeros(sourceImages[i+1].rows, sourceImages[i+1].cols, IMREAD_COLOR);

	remap(sourceImages[i], rectImagel, maplx, maply, INTER_LINEAR);
	remap(sourceImages[i+1], rectImager, maprx, mapry, INTER_LINEAR);
	
	imshow("sourceImage1", sourceImages[i]);
	imshow("sourceImage2", sourceImages[i+1]);
	imshow("rectification left", rectImagel);
	imshow("rectification right", rectImager);
	waitKey(0);


#pragma endregion 
	*/

#pragma region PART 2C
	// PART 2c) get disparity images
	
/*
	#pragma region testing
	
	// lets try already rectified images:
	Mat ILeft = imread(sourcePaths[2] + "im0.png", IMREAD_COLOR);
	Mat IRight = imread(sourcePaths[2]+ "im1.png", IMREAD_COLOR);
	Mat KLeft = (Mat_<double>(3, 3) << 4161.221, 0, 1445.577, 0, 4161.221, 984.686, 0, 0, 1);
	Mat KRight = (Mat_<double>(3, 3) << 4161.221, 0, 1654.636, 0, 4161.221, 984.686, 0, 0, 1);

	// resize images, since they are huge!
	Mat ILeftSmall, IRightSmall;
	resize(ILeft, ILeftSmall, ILeft.size() / 3);
	resize(IRight, IRightSmall, IRight.size() / 3);
	// create empty disparity matrices
	Mat IDisparity;
	Mat IDisparityNormalized;

	// create the disparityMachine and adjust parameters
	Ptr<StereoSGBM> disparityMachine = StereoSGBM::create(0, 272, 5);
	// use params from http://www.jayrambhia.com/blog/disparity-mpas
	disparityMachine->setP1(8 * 3 * 5 * 5);
	disparityMachine->setP2(32 * 3 * 5 * 5);
	disparityMachine->setDisp12MaxDiff(10);
	disparityMachine->setPreFilterCap(4);
	disparityMachine->setUniquenessRatio(5); //openCv doc says 5-15 is good value
	disparityMachine->setSpeckleWindowSize(150);
	disparityMachine->setSpeckleRange(2);
	disparityMachine->setMode(StereoSGBM::MODE_HH); //this uses a LOT of memory
	
	// prepare filter:
//	Ptr<DisparityWLSFilter disparityFilter = create

	disparityMachine->compute(ILeftSmall, IRightSmall, IDisparity);
	normalize(IDisparity, IDisparityNormalized, 0, 255, CV_MINMAX, CV_8U);
	
	// save disparity map:
	imwrite("../../outputFiles/disparityTest.png", IDisparityNormalized);

	//show result
	imshow("DisparityMap", IDisparityNormalized);
	waitKey(0);
	
	
	#pragma endregion
	*/
#pragma endregion 
#pragma region Depth
	/*
	// PART 2d) compute depth images
	// since I don't have a Q matrix in this case, I do it by hand:
	double f = 4161.221;	//focal length in pixels
	double b = 176.252;		// baseline in pixels?
	// read disparity-image
	Mat IDisparity = imread("../../outputFiles/disparityTest.png", CV_8U);
	// create empty depth-image
	Mat IDepth(IDisparity.rows, IDisparity.cols, DataType<float>::type);
	Mat IDepthNormalized;
	//loop over entire image
	for (int i = 0; i < IDisparity.rows; i++) {
		for (int j = 0; j < IDisparity.cols; j++) {
			int d = IDisparity.at<uchar>(i, j);	// disparity value
			int depthValue = 0;
			if (d > 0)
				depthValue = b * f / d;
			// write depth value to depth-image:
			//cout << "(" << i << "," << j << ")" << endl;
			IDepth.at<float>(i, j) = depthValue;
		}
	}
	normalize(IDepth, IDepthNormalized, 0, 255, CV_MINMAX, CV_8U);

	// save result:
	imwrite("../../outputFiles/testDepthLeft.png", IDepthNormalized);

	// show results:
	imshow("Depth-image", IDepthNormalized);
	waitKey(0);
	*/
#pragma endregion 
	//PART 2 FINISHED%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion

#pragma region PART 3
	// PART 3: FUSION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// output: point-cloud, NEEDS TO BE SPECIFIED MORE

#pragma region testing Voxel-Grid
	// lets create a meshed-3D-cube (=voxel grid) and fill the boundary voxels for testing purposes
	// lets use a triple-vector construction, lets say voxelGrid[0,1,2] corresponds to x = 1, y = 2, z = 3
	vector<vector<vector<bool>>> voxelGrid;// (100, vector<vector<double>>(100, vector<double>(100)));
	voxelGrid.resize(100, vector<vector<bool>>(100, vector<bool>(100, false)) );
	// lets fill the boundary voxels with true:
	for (int i = 0; i < voxelGrid.size(); i++) {
		for (int j = 0; j < voxelGrid[0].size(); j++) {
			for (int k = 0; k < voxelGrid[0][0].size(); k += voxelGrid[0][0].size() - 1) {
				voxelGrid[i][j][k] = true;
			}
		}
	}
	for (int i = 0; i < voxelGrid.size(); i++) {
		for (int j = 0; j < voxelGrid[0].size(); j += voxelGrid[0].size() - 1 ) {
			for (int k = 0; k < voxelGrid[0][0].size(); k++) {
				voxelGrid[i][j][k] = true;
			}
		}
	}
	for (int i = 0; i < voxelGrid.size(); i+= voxelGrid.size()-1) {
		for (int j = 0; j < voxelGrid[0].size(); j++) {
			for (int k = 0; k < voxelGrid[0][0].size(); k++) {
				voxelGrid[i][j][k] = true;
			}
		}
	}
	// lets test some values:
	cout << "this values should all be true/1:  " << voxelGrid[40][20][0] << ", " << voxelGrid[0][12][54] << ", " << voxelGrid[16][0][95] << endl;
	cout << "this values should all be false/0: " << voxelGrid[40][20][1] << ", " << voxelGrid[87][12][54] << ", " << voxelGrid[16][1][95] << endl;

	// lets convert the voxel grid into a point cloud:
	// lets say the voxel grid is 1x1x1 in total (of whatever unit)
	vector<Point3f> pointCloud;
	for (int i = 0; i < voxelGrid.size(); i++) {
		for (int j = 0; j < voxelGrid[0].size(); j++) {
			for (int k = 0; k < voxelGrid[0].size(); k++){
				if (voxelGrid[i][j][k] == true) {
					Point3f p(i / 100., j/100., k/100.);
					pointCloud.push_back(p);
				}

			}
		}
	}
	// lets write the pointCloud into a txt file:
	ofstream myfile;
	myfile.open("../../outputFiles/PointCloud.txt");
	for (int i = 0; i < pointCloud.size(); i++) {
		myfile << pointCloud[i].x << ' ' <<  pointCloud[i].y << ' ' << pointCloud[i].z;
		myfile << endl;
	}
	 
	
	
	
	
	myfile.close();

#pragma endregion 



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
// reads cameraParameters and calculates the cameraMatrix K, Orientation R and Translation t w.r.t. the inertial frame for each camera)
void getCameraMatrix(vector<Mat> &K, vector<Mat> &R, vector<Mat> &t) {
	ifstream paramFile(sourcePaths[nrOfPath] + "_par.txt");
	string textLine;
	getline(paramFile, textLine);
	while (getline(paramFile, textLine)) {
		// create matrices and fill them with 0
		Mat Ki = Mat::zeros(3, 3, CV_64F), Ri = Mat::zeros(3, 3, CV_64F), ti = Mat::zeros(3,1, CV_64F);
		// read all parameters from textLine
		istringstream buf(textLine);
		string word;
		for (int i = 0; buf >> word; ++i) {
			if (i == 0) {
				continue;
			}
			else if (i < 10) {
				int j = i - 1;
				Ki.at<double>(j / 3, j % 3) = stod(word);
			}
			else if (i < 19) {
				int j = i - 1 - 9;
				Ri.at<double>(j / 3, j % 3) = stod(word);
			}
			else if (i <= 21) {
				int j = i - 1 - 9 - 9;
				ti.at<double>(j, 0) = stod(word);
			}
			else cout << "Error when parsing cameraParameters" << endl;
		}
		
		R.push_back(Ri);
		t.push_back(ti);
		K.push_back(Ki);
	}
}
