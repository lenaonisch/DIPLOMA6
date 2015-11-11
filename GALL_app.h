#include "stdafx.h"
#include "CRForestDetector.h"
#pragma once

using namespace std;

struct GALL_app
{
public:
	string configpath; // path to config.txt
	string treepath;// Path to trees
	int ntrees;// Number of trees
	int p_width;// Patch width
	int p_height;// Patch height
	string impath;// Path to images
	string imfiles;// File with names of images
	bool xtrFeature;// Extract features
	vector<float> scales;// Scales
	vector<float> ratios;// Ratio
	string outpath;	// Output path
	int out_scale; // scale factor for output image (default: 128)
	string trainpospath; 	// Path to positive examples
	string trainposfiles;	// File with postive examples
	int subsamples_pos;	// Subset of positive images -1: all images
	unsigned int samples_pos; 	// Sample patches from pos. examples
	string trainnegpath;	// Path to positive examples
	string trainnegfiles; // File with postive examples
	int subsamples_neg;// Subset of neg images -1: all images
	unsigned int samples_neg; // Samples from pos. examples

	// offset for saving tree number
	int off_tree;

	GALL_app();
	~GALL_app(void);

	void loadConfig(string filename/*, int mode*/);
	void run_train();
	void run_detect();
	void extract_Patches(CRPatch& Train, CvRNG* pRNG);
	void detect(CRForestDetector& crDetect);
	void show();
	void loadTrainNegFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox);
	void loadTrainPosFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox, std::vector<std::vector<cv::Point> >& vCenter);
	void loadImFile(std::vector<string>& vFilenames);

	void createDirectory(string path)
	{
		replace (path, "/", "\\\\"); 
		system (path.c_str());
	}

private:
	void replace(string& str, string what, string to_what)
	{
		int to_what_len = to_what.length();
		int what_len = what.length();
		int index = 0;
		while (true) {
		 /* Locate the substring to replace. */
		 index = str.find(what, index);
		 if (index == std::string::npos) break;

		 /* Make the replacement. */
		 str.replace(index, what_len, to_what);

		 /* Advance index forward so the next iteration doesn't pick it up as well. */
		 index += to_what_len;
		}
	}

};

