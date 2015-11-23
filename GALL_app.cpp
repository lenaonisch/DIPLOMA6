#pragma once
#include "stdafx.h"
#include "GALL_app.h"
#include "CRForestDetector.h"
#include "HierarhicalThreshold.h"
#include <QTime>

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

	// Init random generator
	//QTime t = QTime::currentTime();
	//int seed = (int)t;

	CvRNG cvRNG(cvGetTickCount ());
						
	// Create directory
	string tpath(configpath+treepath);
	tpath.erase(tpath.find_last_of(PATH_SEP)); //delete fileprefix
	//createDirectory(path);
	QString q_path(tpath.c_str());
	QDir dir(q_path);
	dir.mkpath(dir.absolutePath());

	// Init training data
	CRPatch Train(&cvRNG, p_width, p_height, num_of_classes); 
			
	// Extract training patches
	extract_Patches(Train, &cvRNG); 

	// Train forest
	crForest.trainForest(20, 15, &cvRNG, Train, binary_tests_iterations);

	// Save forest
	string spath(configpath);
	spath += treepath;
	crForest.saveForest(spath.c_str(), off_tree);
}

// Init and start detector
void GALL_app::run_detect(QHash <QString, Results>& results) {
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
	detect(crDetect, results);
}

// load test image filenames
void GALL_app::loadImFile(QHash <QString, Results>& results) {
	
	char buffer[400];

	string path(configpath);
	path += imfiles;
	ifstream in(path.c_str());
	if(in.is_open()) {

		unsigned int size;
		in >> size; //size = 10;
		in.getline(buffer,400); 
		//vFilenames.resize(size);

		for(unsigned int i=0; i<size; ++i) {
			in.getline(buffer,400); 
			results.insert(QString(buffer), Results());
			//vFilenames[i] = buffer;	
		}

	} else {
		string s ("File not found ");
		throw  string_exception(s+imfiles);
	}

	in.close();
}

// load positive training image filenames
void GALL_app::loadTrainPosFile(std::vector<string>& vFilenames, 
								std::vector<cv::Rect>& vBBox, 
								std::vector<cv::Point>& vCenter,
								std::vector<unsigned int> & vClassNums) {

	unsigned int size; 
	string sfiles (configpath);
	sfiles+=trainposfiles;
	ifstream in(sfiles.c_str());

	if(in.is_open()) {
		in >> size;

		vFilenames.resize(size);
		vCenter.resize(size);
		vBBox.resize(size);
		vClassNums.resize(size);

		for(unsigned int i = 0; i<size; i++) {
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
			in >> vCenter[i].x;
			in >> vCenter[i].y;

			// Read class number
			in >> vClassNums[i];
			
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
void GALL_app::detect(CRForestDetector& crDetect, QHash <QString, Results>& results) {

	// Load image names
	loadImFile(results);
				
	char buffer[200];

	//ofstream outputFile;// ((configpath + outpath + "scope.txt").c_str());
	//outputFile.open(configpath + outpath + "scope.txt");
	//bool bOpen = outputFile.is_open();

	// Run detector for each image

	for (QHash<QString, Results>::const_iterator i = results.constBegin(); i != results.constEnd(); ++i)
	{
		// Storage for output
		vector<vector<cv::Mat> > vImgDetect(scales.size());	
		// Load image
		cv::Mat img;
		string filename (configpath + impath + "/" + i.key().toStdString());	
		img = cv::imread(filename.c_str(),CV_LOAD_IMAGE_COLOR);
		if(!img.data) {
			string s ("Could not load image file: " + imfiles + "/" + i.key().toStdString());
			throw  string_exception(s);
		}
		//if (bOpen){
		//	outputFile << i.key().toStdString() << "\n";
		//}

		// Prepare scales and classes
		
		for(unsigned int k = 0; k < vImgDetect.size(); k++) {
			vImgDetect[k].resize(num_of_classes);
			for(unsigned int c = 0; c < vImgDetect[k].size(); c++) {
				vImgDetect[k][c].create ( cvSize(int(img.cols*scales[k]+0.5),int(img.rows*scales[k]+0.5)), CV_32FC1 );
			}
		}

		// Store result
		for(unsigned int k = 0; k < vImgDetect.size(); k++) {

			// Detection for all scales and classes
			crDetect.detectPyramid(img, vImgDetect, ratios);

			cv::Mat tmp ( vImgDetect[k][0].rows, vImgDetect[k][0].cols, CV_8UC1);
			for(unsigned int c=0;c<vImgDetect[k].size(); ++c) {
				
				vImgDetect[k][c].convertTo(tmp, CV_8UC1, out_scale);
				sprintf_s(buffer,"%s/detect-%d_sc%d_c%d.png",(configpath + outpath).c_str(),i,k,c);
				imwrite( buffer, tmp );
				
				
				cv::Mat detectedMax;
				std::vector<cv::Point> maxs;
				if (localMaxima(tmp, detectedMax, 11, maxs))
				{
					sprintf_s(buffer,"%s/detect_max-%d_sc%d_c%d.png",(configpath + outpath).c_str(),i,k,c);
					imwrite( buffer, detectedMax );
					//if (bOpen)
					//{
					//	for (std::vector<cv::Point>::iterator it = maxs.begin() ; it != maxs.end(); ++it)
					//		outputFile << "[" << it->x << "; " << it->y << "]\n";
					//}
				}

				detectedMax.release();

				vImgDetect[k][c].release();
				
			}
			tmp.release();
		}

		// Release image
		img.release();

	}
	//outputFile.close();
}

// Extract patches from training data
void GALL_app::extract_Patches(CRPatch& Train, CvRNG* pRNG) {
		
	vector<string> vFilenames;
	vector<cv::Rect> vBBox;
	vector<cv::Point> vCenter;
	vector<unsigned int> vClassNums; // to what class object belongs

	// load positive file list
	loadTrainPosFile(vFilenames, vBBox, vCenter, vClassNums);

	// load postive images and extract patches
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
			Train.extractPatches(img, samples_pos, vClassNums[i], &vBBox[i], &vCenter[i]); 

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
				Train.extractPatches(img, samples_neg, num_of_classes, &vBBox[i]); // enumeration from 0, so index num_of_classes - index of background!
			else
				Train.extractPatches(img, samples_neg, num_of_classes); 

			// Release image
			img.release();

		}
			
	}
}

bool GALL_app::localMaxima(cv::Mat src,cv::Mat &dst,int squareSize, std::vector<cv::Point>& locations)
{
	if (squareSize==0)
	{
		dst = src.clone();
		return false;
	}

	cv::Mat m0;
	dst = src.clone();
	cv::Point maxLoc(0,0);

	//1.Be sure to have at least 3x3 for at least looking at 1 pixel close neighbours
	//  Also the window must be <odd>x<odd>
	//SANITYCHECK(squareSize,3,1);
	int sqrCenter = (squareSize-1)/2;
	if (squareSize%2 == 0)
	{
		squareSize--;
	}


	//2.Create the localWindow mask to get things done faster
	//  When we find a local maxima we will multiply the subwindow with this MASK
	//  So that we will not search for those 0 values again and again
	cv::Mat localWindowMask = cv::Mat::zeros(cv::Size(squareSize,squareSize),CV_8U);//boolean
	localWindowMask.at<unsigned char>(sqrCenter,sqrCenter)=1;

	//3.Find the threshold value to threshold the image
		//this function here returns the peak of histogram of picture
		//the picture is a thresholded picture it will have a lot of zero values in it
		//so that the second boolean variable says :
		//  (boolean) ? "return peak even if it is at 0" : "return peak discarding 0"
	int thrshld =  maxUsedValInHistogramData(dst);
	if (thrshld == 0) //black image, no max
		return false;
	threshold(dst, m0, thrshld, 1, 3); // 5th parameter 0 === THRESH_BINARY

	//4.Now delete all thresholded values from picture
	dst = dst.mul(m0);

	//put the src in the middle of the big array
	for (int row=sqrCenter;row<dst.size().height-sqrCenter;row++)
	{
		for (int col=sqrCenter;col<dst.size().width-sqrCenter;col++)
			{
			//1.if the value is zero it can not be a local maxima
			if (dst.at<unsigned char>(row,col)==0)
				continue;
			//2.the value at (row,col) is not 0 so it can be a local maxima point
			m0 =  dst.colRange(col-sqrCenter,col+sqrCenter+1).rowRange(row-sqrCenter,row+sqrCenter+1);
			minMaxLoc(m0,NULL,NULL,NULL,&maxLoc);
			//if the maximum location of this subWindow is at center
			//it means we found the local maxima
			//so we should delete the surrounding values which lies in the subWindow area
			//hence we will not try to find if a point is at localMaxima when already found a neighbour was
			if ((maxLoc.x==sqrCenter)&&(maxLoc.y==sqrCenter))
			{
				locations.push_back(cv::Point(col, row));
				m0 = m0.mul(localWindowMask);
								//we can skip the values that we already made 0 by the above function
				col+=sqrCenter;
			}
		}
	}
	return true;
}

int GALL_app::maxUsedValInHistogramData(cv::Mat src)
{
	HierarhicalThreshold ht(src);
	int bins = 10;
	if (ht.ProcessImage(1, bins))
		return 255/bins*ht.thresholds_for_max_sigma[0];
	return 0;
}

GALL_app::~GALL_app(void)
{
}