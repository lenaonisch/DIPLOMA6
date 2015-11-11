#pragma once
#include "stdafx.h"
#include "GALL_app.h"
#include "CRForestDetector.h"
#include <QTime>

#define PATH_SEP "/"
using namespace std;

GALL_app::GALL_app()
{

}

void GALL_app::loadConfig(string filename/*, int mode*/)
{
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
		//in.getline(buffer,400);

	} else {
		string s ("File not found ");
		s += filename;
		throw  string_exception(s);
	}
	in.close();

	/*switch ( mode ) { 
		case 0:
		cout << endl << "------------------------------------" << endl << endl;
		cout << "Training:         " << endl;
		cout << "Patches:          " << p_width << " " << p_height << endl;
		cout << "Train pos:        " << trainpospath << endl;
		cout << "                  " << trainposfiles << endl;
		cout << "                  " << subsamples_pos << " " << samples_pos << endl;
		cout << "Train neg:        " << trainnegpath << endl;
		cout << "                  " << trainnegfiles << endl;
		cout << "                  " << subsamples_neg << " " << samples_neg << endl;
		cout << "Trees:            " << ntrees << " " << off_tree << " " << treepath << endl;
		cout << endl << "------------------------------------" << endl << endl;
		break;

		case 1:
		cout << endl << "------------------------------------" << endl << endl;
		cout << "Show:             " << endl;
		cout << "Trees:            " << ntrees << " " << treepath << endl;
		cout << endl << "------------------------------------" << endl << endl;
		break;

		default:
		cout << endl << "------------------------------------" << endl << endl;
		cout << "Detection:        " << endl;
		cout << "Trees:            " << ntrees << " " << treepath << endl;
		cout << "Patches:          " << p_width << " " << p_height << endl;
		cout << "Images:           " << impath << endl;
		cout << "                  " << imfiles << endl;
		cout << "Scales:           "; for(unsigned int i=0;i<scales.size();++i) cout << scales[i] << " "; cout << endl;
		cout << "Ratios:           "; for(unsigned int i=0;i<ratios.size();++i) cout << ratios[i] << " "; cout << endl;
		cout << "Extract Features: " << xtrFeature << endl;
		cout << "Output:           " << out_scale << " " << outpath << endl;
		cout << endl << "------------------------------------" << endl << endl;
		break;*/
	//}
}

void GALL_app::run_train()
{
		// Init forest with number of trees
	CRForest crForest( ntrees ); 

	// Init random generator
	//QTime t = QTime::currentTime();
	//int seed = (int)t;

	CvRNG cvRNG(cvGetTickCount ());
						
	// Create directory
	string tpath(configpath);
	//string tpath(treepath);
	tpath += treepath;
	tpath.erase(tpath.find_last_of(PATH_SEP)); //delete fileprefix
	//createDirectory(path);
	QString q_path(tpath.c_str());
	QDir dir(q_path);
	dir.mkpath(dir.absolutePath());

	// Init training data
	CRPatch Train(&cvRNG, p_width, p_height, 2); 
			
	// Extract training patches
	extract_Patches(Train, &cvRNG); 

	// Train forest
	crForest.trainForest(20, 15, &cvRNG, Train, 2000);

	// Save forest
	string spath(configpath);
	spath += treepath;
	crForest.saveForest(spath.c_str(), off_tree);
}

// Init and start detector
void GALL_app::run_detect() {
	// Init forest with number of trees
	CRForest crForest( ntrees ); 

	// Load forest
	string fpath(configpath);
	fpath += treepath;
	crForest.loadForest(fpath.c_str());	

	// Init detector
	CRForestDetector crDetect(&crForest, p_width, p_height);

	// create directory for output
	string path(configpath);
	path += outpath;
	QString q_path(path.c_str());
	QDir dir(q_path);
	dir.mkpath(dir.absolutePath());

	// run detector
	detect(crDetect);
}

// load test image filenames
void GALL_app::loadImFile(std::vector<string>& vFilenames) {
	
	char buffer[400];

	string path(configpath);
	path += imfiles;
	ifstream in(path.c_str());
	if(in.is_open()) {

		unsigned int size;
		in >> size; //size = 10;
		in.getline(buffer,400); 
		vFilenames.resize(size);

		for(unsigned int i=0; i<size; ++i) {
			in.getline(buffer,400);      
			vFilenames[i] = buffer;	
		}

	} else {
		string s ("File not found ");
		throw  string_exception(s+imfiles);
	}

	in.close();
}

// load positive training image filenames
void GALL_app::loadTrainPosFile(std::vector<string>& vFilenames, std::vector<cv::Rect>& vBBox, std::vector<std::vector<cv::Point> >& vCenter) {

	unsigned int size, numop; 
	string sfiles (configpath);
	sfiles+=trainposfiles;
	ifstream in(sfiles.c_str());

	if(in.is_open()) {
		in >> size;
		in >> numop;
		cout << "Load Train Pos Examples: " << size << " - " << numop << endl;

		vFilenames.resize(size);
		vCenter.resize(size);
		vBBox.resize(size);

		for(unsigned int i=0; i<size; ++i) {
			// Read filename
			in >> vFilenames[i];

			// Read bounding box
			in >> vBBox[i].x; in >> vBBox[i].y; 
			in >> vBBox[i].width;
			vBBox[i].width -= vBBox[i].x; 
			in >> vBBox[i].height;
			vBBox[i].height -= vBBox[i].y;

			if(vBBox[i].width<p_width || vBBox[i].height<p_height) {
				throw string_exception("Width or height are too small in file" + vFilenames[i]);  
			}

			// Read center points
			vCenter[i].resize(numop);
			for(unsigned int c=0; c<numop; ++c) {			
				in >> vCenter[i][c].x;
				in >> vCenter[i][c].y;
			}				
		}

		in.close();
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
void GALL_app::detect(CRForestDetector& crDetect) {

	// Load image names
	vector<string> vFilenames;
	loadImFile(vFilenames);
				
	char buffer[200];

	// Storage for output
	vector<vector<cv::Mat> > vImgDetect(scales.size());	

	// Run detector for each image
	for(unsigned int i=0; i<vFilenames.size(); ++i) {

		// Load image
		cv::Mat img;
		img = cv::imread((configpath + impath + "/" + vFilenames[i]).c_str(),CV_LOAD_IMAGE_COLOR);
		if(!img.data) {
			string s ("Could not load image file: ");
			s += imfiles;
			s += "/";
			s += vFilenames[i];
			throw  string_exception(s);
		}	

		// Prepare scales
		for(unsigned int k=0;k<vImgDetect.size(); ++k) {
			vImgDetect[k].resize(ratios.size());
			for(unsigned int c=0;c<vImgDetect[k].size(); ++c) {
				vImgDetect[k][c].create ( cvSize(int(img.cols*scales[k]+0.5),int(img.rows*scales[k]+0.5)), CV_32FC1 );
			}
		}

		// Detection for all scales
		crDetect.detectPyramid(img, vImgDetect, ratios);

		// Store result
		for(unsigned int k=0;k<vImgDetect.size(); ++k) {
			cv::Mat tmp ( vImgDetect[k][0].rows, vImgDetect[k][0].cols, CV_8UC1);
			for(unsigned int c=0;c<vImgDetect[k].size(); ++c) {
				
				vImgDetect[k][c].convertTo(tmp, CV_8UC1, out_scale);
				sprintf_s(buffer,"%s/detect-%d_sc%d_c%d.png",(configpath + outpath).c_str(),i,k,c);
				imwrite( buffer, tmp );
								
				vImgDetect[k][c].release();
				
			}
			tmp.release();
		}

		// Release image
		img.release();

	}

}

// Extract patches from training data
void GALL_app::extract_Patches(CRPatch& Train, CvRNG* pRNG) {
		
	vector<string> vFilenames;
	vector<cv::Rect> vBBox;
	vector<vector<cv::Point> > vCenter;

	// load positive file list
	loadTrainPosFile(vFilenames,  vBBox, vCenter);

	// load postive images and extract patches
	for(int i=0; i<(int)vFilenames.size(); ++i) 
	{

	  if(i%50==0) cout << i << " " << flush;

	  if(subsamples_pos <= 0 || (int)vFilenames.size()<=subsamples_pos || (cvRandReal(pRNG)*double(vFilenames.size()) < double(subsamples_pos)) ) {

			// Load image
			cv::Mat img;
			string sfile (configpath+trainpospath + "/" + vFilenames[i]);
			img = cv::imread(sfile.c_str(),CV_LOAD_IMAGE_COLOR);
			if(!img.data) {
				throw string_exception("Could not load image file: " + sfile +" ! Check config.txt");
			}	

			// Extract positive training patches
			Train.extractPatches(img, samples_pos, 1, &vBBox[i], &vCenter[i]); 

			// Release image
			img.release();

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
				Train.extractPatches(img, samples_neg, 0, &vBBox[i]); 
			else
				Train.extractPatches(img, samples_neg, 0); 

			// Release image
			img.release();

		}
			
	}
}

GALL_app::~GALL_app(void)
{
}