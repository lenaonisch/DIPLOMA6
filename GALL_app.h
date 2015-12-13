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
	string train_rescaled_cropped_path;
	string trainposfiles;	// File with postive examples
	int subsamples_pos;	// Subset of positive images -1: all images
	unsigned int samples_pos; 	// Sample patches from pos. examples
	string trainnegpath;	// Path to positive examples
	string trainnegfiles; // File with postive examples
	int subsamples_neg;// Subset of neg images -1: all images
	unsigned int samples_neg; // Samples from pos. examples
	unsigned int binary_tests_iterations;
	string classmap_file;
	int num_of_classes; //number of classes
	vector<string> classes; //class names
	vector<int> width_aver; // average width of training images for each class
	vector<int> height_min; // minimum height detected on training images for each class

	// offset for saving tree number
	int off_tree;

	CRForestDetector crDetect;
	CRForest crForest;

	GALL_app();
	~GALL_app(void);

	void loadConfig(string filename/*, int mode*/);
	void loadForest();
	void run_train();
	void run_detect(bool& load_forest, map<string, Results>& results);
	void run_detect(bool& load_forest, string filename, Results& results);
	void extract_Patches(CRPatch& Train, CvRNG* pRNG);
	void detect(CRForestDetector& crDetect, vector<std::string> filenames, vector<Results>& results);
	void show();
	void loadTrainNegFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox);
	void loadTrainPosFile(std::vector<string>& vFilenames, 
						  std::vector<cv::Rect>& vBBox, 
						  /*std::vector<cv::Point>& vCenter,*/
						  std::vector<unsigned int> & vClassNums,
						  std::vector<int>& width_aver);
	void loadImFile(vector<std::string>& filenames);

	void createDirectory(string path)
	{
		replace (path, "/", "\\\\"); 
		system (path.c_str());
	}

	string getFilename(string filepath){
		std::size_t found = filepath.find_last_of("/");
		return filepath.substr(found+1);	
	}


	void read_classes()
	{
		char buffer[400];
		ifstream in_class(configpath+classmap_file);
		if(in_class.is_open()) {
			in_class >> num_of_classes;
			classes.resize(num_of_classes);
			for (int i = 0; i < num_of_classes; i++)
				in_class >> buffer >> classes[i];
		}
		else {
			string s ("Classmap file not found ");
			s += classmap_file;
			throw  string_exception(s);
		}
		in_class.close();
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

