/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#include "CRForest.h"
#include "HierarhicalThreshold.h"
#include "AMP_char.h"

class CRForestDetector {
public:
	// Constructor
	CRForestDetector(const CRForest* pRF, TestParam testParam, const char* filename) : 
				crForest(pRF), testParam(testParam)
	{
		//width_aver = *aver_width; // will be read from file
		//height_min = *_height_min;
		load(filename);

	}

	CRForestDetector(const CRForest* pRF, int w, int h, TestParam testParam, vector<int>* aver_width, vector<int>* _height_min) : 
				crForest(pRF), width(w), height(h), testParam(testParam)
	{
		width_aver = *aver_width;
		height_min = *_height_min;
	}

	CRForestDetector(){}

	CRForestDetector& operator=(CRForestDetector& right) { // copy/move constructor is called to construct arg
		if (this == &right) {
            return *this;
        }
        
		crForest = right.crForest;
		width = right.width;
		height = right.height;
		testParam = right.testParam;
		width_aver = right.width_aver;
		height_min = right.height_min;
		return *this;
	}

	// detect multi scale
	void detectPyramid(cv::Mat img, vector<float>& scales, vector<vector<cv::Mat> >& imgDetect, Results& result);
	int maxUsedValInHistogramData(cv::Mat src);
	bool localMaxima(cv::Mat src, cv::Size size, vector<MaxPoint>& locations, int class_label, int threshold);
	void ConvertToTesults(vector<MaxPoint>& maxs,Results& result);

	void save(const char* filename) {
		ofstream out(filename);
		if(out.is_open()) {
			//out << "# Number of classes" << endl;
			//out << class_count << endl;
			//out << "# Width & height of patch" << endl;
			out << width << " " << height << " " << endl;
			//out << "# Out scale (copied from config.txt)" << endl;
			//out << out_scale << endl;
			//out << "# Threshold to make leaf vote for particular class center" << endl;
			out << testParam.prob_threshold << endl;
			//out << "# Width for classes" << endl;
			for (int i=0;i<num_of_classes; i++)
				out << width_aver[i]<<" ";
			//out <<endl << "# height_min"<< endl;
			out << endl;
			for (int i=0;i<num_of_classes; i++)
				out << height_min[i]<<" ";
		}
	}

	void load(const char* filename)
	{
		FILE * pFile = fopen (filename,"r");
		if (pFile != NULL)
		{
			//fscanf (pFile, "%i", &class_count);
			fscanf (pFile, "%i %i", &width, &height);
			//fscanf (pFile, "%i", &out_scale);
			fscanf (pFile, "%f", &testParam.prob_threshold);
			width_aver.resize(num_of_classes);
			height_min.resize(num_of_classes);
			for (int i = 0; i < num_of_classes; i++)
				fscanf (pFile, "%i", &width_aver[i]);
			for (int i = 0; i < num_of_classes; i++)
				fscanf (pFile, "%i", &height_min[i]);
		}
	}
	void convertToMultiChannel(concurrency::array_view<unsigned int>& outputView, vector<cv::Mat> input);

	//int*** treetable;
	//unsigned int** treepointer;

	//float** leafs;
	//unsigned int** leafpointer;
	TestParam testParam;
private:
	// returns time
	int detectColor(cv::Mat img, cv::Size size, cv::Mat& imgDetect, cv::Mat& ratios); 
	const CRForest* crForest;
	int width;
	int height;
	
	vector<int> width_aver;
	vector<int> height_min;
};
