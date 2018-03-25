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
// basic opencv functionalities
#include <opencv2/opencv.hpp>	
#include "opencv2/highgui.hpp"	// reading and writing images
#include <opencv2/core/core.hpp>// displaying images with imshow

#pragma endregion INCLUDES

using namespace std; // don't need to write std::
using namespace cv;

#pragma region FUNCTION-DEFINITIONS
// no functions yet
#pragma endregion FUNCTION-DEFINITIONS

#pragma region GLOBAL-VARIABLES
//source images (color images that we take) are saved in this vector
vector<Mat> sourceImages;
// array that holds strings where we load the images from
string sourcePaths[1] = {
	"../../DataSets/MultiViewImages/templeDataSet/templeSparseRing/templeSparseRing/templeSR"
};
// number indicating which sourcePath we actually want to select
int nrOfPath = 0;
#pragma endregion GLOBAL-VARIABLES



int main()
{
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
	// output: death maps, NEEDS TO BE SPECIFIED MORE
	// PART 2a) use Ri and Ci for pair-wise rectification.
	// Read test-images and save them into a vector:
	for (int i = 1; i <= 16; i++) {
		char number[5];
		snprintf(number, 5, "%04d", i);
		sourceImages.push_back(imread(sourcePaths[nrOfPath] + number + ".png", IMREAD_COLOR));
	}
	// Read camera calibration parameters and save them:


	// PART 2b) do pair-wise triangulation (dense-stereo)


	
	//PART 2 FINISHED%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion PART 2

#pragma region PART 3
	// PART 3: FUSION %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// output: point-cloud, NEEDS TO BE SPECIFIED MORE

	// PART 3 FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion PART 3

#pragma region PART 4
	// PART 4: MESHING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// PART 4 FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion PART 4 

#pragma region PART 5
	// PART 5: HOLE CLOSING %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	// PART 5: HOLE CLOSING FINISHED %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#pragma endregion PART 5


    return 0;
}

