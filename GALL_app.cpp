#pragma once
#include "stdafx.h"
#include "GALL_app.h"
#include "CRForestDetector.h"
#include <QTime>

int num_of_classes; //number of classes

#define PATH_SEP "/"
using namespace std;

GALL_app::GALL_app()
{

}

void GALL_app::loadConfig(string filename)
{
	string directory;
	const size_t last_slash_idx = filename.rfind('/');
	if (std::string::npos != last_slash_idx)
	{
		configpath = filename.substr(0, last_slash_idx);
	}

	char buffer[400];
	ifstream in(filename);

	if(in.is_open()) {

		// Path to trees
		in.getline(buffer,400);
		in.getline(buffer,400); 
		treepath = buffer;
		// Number of trees
		in.getline(buffer,400); 
		in >> ntrees;
		in.getline(buffer,400); 
		// Patch width
		in.getline(buffer,400); 
		in >> p_width;
		in.getline(buffer,400); 
		// Patch height
		in.getline(buffer,400); 
		in >> p_height;
		in.getline(buffer,400); 
		// Path to images
		in.getline(buffer,400); 
		in.getline(buffer,400); 
		impath = buffer;
		// File with names of images
		in.getline(buffer,400);
		in.getline(buffer,400);
		imfiles = buffer;
		// Extract features
		in.getline(buffer,400);
		in >> xtrFeature;
		in.getline(buffer,400); 
		// Scales
		in.getline(buffer,400);
		int size;
		in >> size;
		scales.resize(size);
		for(int i=0;i<size;++i)
			in >> scales[i];
		in.getline(buffer,400); 
		// Ratio
		in.getline(buffer,400);
		in >> size;
		ratios.resize(size);
		for(int i=0;i<size;++i)
			in >> ratios[i];
		in.getline(buffer,400); 
		// Output path
		in.getline(buffer,400);
		in.getline(buffer,400);
		outpath = buffer;
		// Scale factor for output image (default: 128)
		in.getline(buffer,400);
		in >> out_scale;
		in.getline(buffer,400);
		// Path to positive examples
		in.getline(buffer,400);
		in.getline(buffer,400);
		trainpospath = buffer;
		// File with postive examples
		in.getline(buffer,400);
		in.getline(buffer,400);
		trainposfiles = buffer;
		// Subset of positive images -1: all images
		in.getline(buffer,400);
		in >> subsamples_pos;
		in.getline(buffer,400);
		// Samples from pos. examples
		in.getline(buffer,400);
		in >> samples_pos;
		in.getline(buffer,400);
		// Path to positive examples
		in.getline(buffer,400);
		in.getline(buffer,400);
		trainnegpath = buffer;
		// File with postive examples
		in.getline(buffer,400);
		in.getline(buffer,400);
		trainnegfiles = buffer;
		// Subset of negative images -1: all images
		in.getline(buffer,400);
		in >> subsamples_neg;
		in.getline(buffer,400);
		// Samples from pos. examples
		in.getline(buffer,400);
		in >> samples_neg;
		// Number of binary tests generated
		in.getline(buffer,400);
		in.getline(buffer,400);
		in >> binary_tests_iterations;
		// path to classmap file
		in.getline(buffer,400);
		in.getline(buffer,400);
		in >> classmap_file;
		read_classes();

		// rescalsed path
		in.getline(buffer, 400);
		in.getline(buffer,400);
		in >> train_rescaled_cropped_path;
		// average width 
		in.getline(buffer, 400);
		in.getline(buffer, 400);
		int tmp_w;
		in >> tmp_w;
		if (tmp_w != 0)
		{
			width_aver.resize(num_of_classes);
			width_aver[0] = tmp_w;
			for (int i = 1; i<num_of_classes; i++)
				in >>width_aver[i];
		}

	} else {
		string s ("Config file not found ");
		s += filename;
		throw  string_exception(s);
	}
	in.close();
}

void GALL_app::run_train()
{
		// Init forest with number of trees
	CRForest crForest( ntrees ); 

	CvRNG cvRNG(cvGetTickCount ());
						
	// Create directory
	string tpath(configpath+treepath);
	tpath.erase(tpath.find_last_of(PATH_SEP)); //delete fileprefix
	QString q_path(tpath.c_str());
	QDir dir(q_path);
	dir.mkpath(dir.absolutePath());

	QString r_path((configpath+train_rescaled_cropped_path).c_str());
	QDir dir_r(r_path);
	dir_r.mkpath(dir_r.absolutePath());

	// Init training data
	CRPatch Train(&cvRNG, p_width, p_height, num_of_classes); 
			
	// Extract training patches
	extract_Patches(Train, &cvRNG);
	CRForestDetector crDetect(&crForest, p_width, p_height, pow(1+num_of_classes, -0.66), &width_aver, &height_min, out_scale);
	crDetect.save((configpath + treepath + "forest_detector.txt").c_str());

	// Train forest
	string spath(configpath);
	spath += treepath;
	off_tree = 0;
	crForest.trainForest(20, 15, &cvRNG, Train, binary_tests_iterations,spath.c_str(), off_tree);

	//// Save forest
	//string spath(configpath);
	//spath += treepath;
	//crForest.saveForest(spath.c_str(), off_tree);
}

// Init and start detector
void GALL_app::run_detect(bool& load_forest, map<string, Results>& results) {
	if (!load_forest)
	{
		loadForest();
		load_forest = true;
	}

	// run detector
	for(map<string, Results>::iterator it = results.begin(); it != results.end(); ++it) {
		if (!it->second.processed)
			detect(crDetect, it->first, it->second);
	}
}

// Init and start detector
void GALL_app::run_detect(bool& load_forest, string filename, Results& results) {
	if (!load_forest)
	{
		loadForest();
		load_forest = true;
	}

	// run detector
	detect(crDetect,  filename, results);
}

void GALL_app::loadForest()
{
	// Init forest with number of trees
	crForest = CRForest ( ntrees ); 

	// Load forest
	string fpath(configpath);
	fpath += treepath;
	crForest.loadForest(fpath.c_str());	

	// Init detector
	crDetect = CRForestDetector(&crForest, p_width, p_height, pow(1+num_of_classes, -0.66), out_scale, (configpath + treepath + "forest_detector.txt").c_str());
	//crDetect.load((configpath + treepath + "forest_detector.txt").c_str());

	// create directory for output
	string path(configpath);
	path += outpath;
	QString q_path(path.c_str());
	QDir dir(q_path);
	dir.mkpath(dir.absolutePath());
}

// load test image filenames
void GALL_app::loadImFile(vector<std::string>& filenames) {
	
	char buffer[400];

	string path(configpath);
	path += imfiles;

	FILE * pFile = fopen (path.c_str(),"r");
	if (pFile != NULL)
	{

		unsigned int size;
		fscanf (pFile, "%i", &size);
		filenames.resize(size);

		for(unsigned int i=0; i<size; ++i) {
			fscanf (pFile, "%s", &buffer);
			string filename = buffer;	
			filenames[i] = configpath + impath + "/" + filename;
		}

	} else {
		throw  string_exception("File not found " + imfiles);
	}

	if (fclose (pFile) != 0)
	{
		throw string_exception("Failed to close " + path);
	}
}

// load positive training image filenames
void GALL_app::loadTrainPosFile(std::vector<string>& vFilenames, 
								std::vector<cv::Rect>& vBBox, 
								/*std::vector<cv::Point>& vCenter,*/
								std::vector<unsigned int> & vClassNums,
								std::vector<int>& width_aver = vector<int>()) {

	unsigned int size; 
	string sfiles (configpath);
	sfiles+=trainposfiles;
	FILE * pFile = fopen (sfiles.c_str(),"r");
	
	if (pFile != NULL)
	{
		fscanf (pFile, "%i", &size);

		bool w_aver = (width_aver.size() == 0);
		vFilenames.resize(size);
		//vCenter.resize(size);
		vBBox.resize(size);
		vClassNums.resize(size);
		if (w_aver)
		{
			width_aver.resize(num_of_classes, 0);
		}
		height_min.resize(num_of_classes, INT_MAX);
		ratios.resize(size);
		vector<int> class_instances(num_of_classes, 0); // number of instances of each class

		for(unsigned int i = 0; i<size; i++) {
			char name [300];
			// Read filename
			fscanf (pFile, " %s %i %i %i %i %i",
					&name, 
					&vBBox[i].x, &vBBox[i].y, 
					&vBBox[i].width, &vBBox[i].height,
					/*&vCenter[i].x, &vCenter[i].y,*/
					&vClassNums[i]);

			vFilenames[i] = name;
			vBBox[i].width -= vBBox[i].x; 
			vBBox[i].height -= vBBox[i].y;
			/*vCenter[i].x = vBBox[i].width/2;
			vCenter[i].y = vBBox[i].height/2;*/

			if(vBBox[i].width<p_width || vBBox[i].height<p_height) {
				throw string_exception("Width or height are too small in file" + vFilenames[i]);  
			}

			int cl = vClassNums[i];
			class_instances[cl]++;
			if (w_aver)
				width_aver[cl] += vBBox[i].width;
			/*if (vBBox[i].height < height_min[cl])
				height_min[cl] = vBBox[i].height;*/
		}

		if (w_aver)
			for (int i = 0; i < num_of_classes; i++)
			{
				width_aver[i] /= class_instances[i];
			}

		if (fclose (pFile) != 0)
		{
			throw string_exception("Failed to close " + sfiles);
		}
	} else {
		throw string_exception("File not found " + sfiles);
	}
}

// load negative training image filenames
void GALL_app::loadTrainNegFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox) {

	unsigned int size, numop; 
	string sfiles (configpath);
	sfiles+=trainnegfiles;
	ifstream in(sfiles.c_str());

	if(in.is_open()) {
		in >> size;
		in >> numop;
		cout << "Load Train Neg Examples: " << size << " - " << numop << endl;

		vFilenames.resize(size);
		if(numop>0)
			vBBox.resize(size);
		else
			vBBox.clear();

		for(unsigned int i=0; i<size; ++i) {
			// Read filename
			in >> vFilenames[i];

			// Read bounding box (if available)
			if(numop>0) {
				in >> vBBox[i].x; in >> vBBox[i].y; 
				in >> vBBox[i].width;
				vBBox[i].width -= vBBox[i].x; 
				in >> vBBox[i].height;
				vBBox[i].height -= vBBox[i].y;

				if(vBBox[i].width<p_width || vBBox[i].height<p_height) {
				  cout << "Width or height are too small" << endl; 
				  cout << vFilenames[i] << endl;
				  exit(-1); 
				}
			}	
		}

		in.close();
	} else {
		throw string_exception("File not found " + trainposfiles);
	}
}

// Show leaves
void GALL_app::show() {
	// Init forest with number of trees
	CRForest crForest( ntrees ); 

	// Load forest
	string tpath(configpath);
	tpath += treepath;
	crForest.loadForest(tpath.c_str());	

	// Show leaves
	crForest.show(100,100);
}

// Run detector
void GALL_app::detect(CRForestDetector& crDetect, string filename, Results& results) {
				
	char buffer[200];

	// Storage for output
	vector<vector<cv::Mat> > vImgDetect(scales.size());	
	// Load image
	cv::Mat img;
	img = cv::imread(filename.c_str(),CV_LOAD_IMAGE_COLOR);
	string short_name, ext;
	getFilenameExt(filename, short_name, ext);
	if(!img.data) {
		string s ("Could not load image file: " + filename);
		throw  string_exception(s);
	}

	// Detection for all scales and classes
	double* timers = crDetect.detectPyramid(img, scales, vImgDetect, results);
	results.processed = true;
	filename = short_name;

	// Store result
	for(unsigned int k = 0; k < vImgDetect.size(); k++) {
		for(unsigned int c = 0; c < vImgDetect[k].size(); ++c) {
				
			//vImgDetect[k][c].convertTo(tmp, CV_8UC1, out_scale);
			// int k - scale, c - index of class
			sprintf_s(buffer,"%s/%s_scale%f_%s.%s",(configpath + outpath).c_str(), short_name.c_str(), scales[k], classes[c].c_str(), ext.c_str());
			imwrite( buffer, vImgDetect[k][c] );

			vImgDetect[k][c].release();
				
		}
	}

	// Release image
	img.release();
}

// Extract patches from training data
void GALL_app::extract_Patches(CRPatch& Train, CvRNG* pRNG) {
		
	vector<string> vFilenames;
	vector<cv::Rect> vBBox;
	vector<cv::Point> vCenter;
	vector<unsigned int> vClassNums; // to what class object belongs

	// load positive file list
	if (width_aver.size() == 0)
		loadTrainPosFile(vFilenames, vBBox, /*vCenter,*/ vClassNums, width_aver);
	else
		loadTrainPosFile(vFilenames, vBBox, /*vCenter,*/ vClassNums); // all images will be resized to one width pointed in config file

	// load postive images and extract patches
	vCenter.resize(vFilenames.size());
	for(int i=0; i<(int)vFilenames.size(); ++i) 
	{
	  if(i%50==0) cout << i << " " << flush;

	  if(subsamples_pos <= 0 || (int)vFilenames.size()<=subsamples_pos || (cvRandReal(pRNG)*double(vFilenames.size()) < double(subsamples_pos)) ) 
	  {
			// Load image
			cv::Mat img;
			string sfile (configpath+trainpospath + "/" + vFilenames[i]);
			img = cv::imread(sfile.c_str(),CV_LOAD_IMAGE_COLOR);
			if(!img.data) {
				throw string_exception("Could not load image file: " + sfile +" ! Check config.txt");
			}	

			// Extract positive training patches
			float dx = width_aver[vClassNums[i]]/(float)vBBox[i].width;
			cv::Mat to_extract;
			cv::resize(img(vBBox[i]), to_extract, cv::Size(), dx, dx);
			vCenter[i].x = to_extract.cols/2;
			vCenter[i].y = to_extract.rows/2;
			if (to_extract.rows < height_min[vClassNums[i]])
				height_min[vClassNums[i]] = to_extract.rows;
			cv::imwrite((configpath + train_rescaled_cropped_path + "/" + vFilenames[i]).c_str(), to_extract);

			Train.extractPatches(to_extract, samples_pos, vClassNums[i], &vCenter[i]); 

			// Release image
			img.release();
			to_extract.release();
	  }		
	}
	cout << endl;

	// load negative file list
	loadTrainNegFile(vFilenames,  vBBox);

	// load negative images and extract patches
	for(int i=0; i<(int)vFilenames.size(); ++i) {

		if(i%50==0) cout << i << " " << flush;

		if(subsamples_neg <= 0 || (int)vFilenames.size()<=subsamples_neg || ( cvRandReal(pRNG)*double(vFilenames.size()) < double(subsamples_neg) ) ) {

			// Load image
			cv::Mat img;
			string sfile(configpath + trainnegpath + "/" + vFilenames[i]);
			img = cv::imread(sfile.c_str(),CV_LOAD_IMAGE_COLOR);

			if(!img.data) {
				throw string_exception("Could not load image file: " + sfile + " ! Check config.txt");
			}	

			// Extract negative training patches
			if(vBBox.size()==vFilenames.size())
				Train.extractPatches(img(vBBox[i]), samples_neg, num_of_classes); // enumeration from 0, so index num_of_classes - index of background!
			else
				Train.extractPatches(img, samples_neg, num_of_classes); 

			// Release image
			img.release();
		}
			
	}
}

GALL_app::~GALL_app(void)
{
}