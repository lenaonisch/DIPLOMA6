/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#include "CRForest.h"
#include "HierarhicalThreshold.h"

class CRForestDetector {
public:
	// Constructor
	CRForestDetector(const CRForest* pRF, int w, int h, float prob_t, vector<int>* aver_width, vector<int>* _height_min, int _out_scale) : 
				crForest(pRF), width(w), height(h), prob_threshold(prob_t)
	{
		class_count = aver_width->size();
		width_aver = *aver_width;
		height_min = *_height_min;
		out_scale = _out_scale;
	}

	CRForestDetector(){}

	CRForestDetector& operator=(CRForestDetector& right) { // copy/move constructor is called to construct arg
		if (this == &right) {
            return *this;
        }
        
		crForest = right.crForest;
		width = right.width;
		height = right.height;
		prob_threshold = right.prob_threshold;
		width_aver = right.width_aver;
		height_min = right.height_min;
		class_count = right.class_count;
		out_scale = right.out_scale;

		return *this;
	}

	// detect multi scale
	void detectPyramid(cv::Mat img, vector<float>& scales, vector<vector<cv::Mat> >& imgDetect, Results& result);
	int maxUsedValInHistogramData(cv::Mat src);
	bool localMaxima(cv::Mat src,cv::Mat &dst, cv::Size size, vector<MaxPoint>& locations, int class_label);

	void save(const char* filename) {
		ofstream out(filename);
		if(out.is_open()) {
			//out << "# Number of classes" << endl;
			out << class_count << endl;
			//out << "# Width & height of patch" << endl;
			out << width << " " << height << " " << endl;
			//out << "# Out scale (copied from config.txt)" << endl;
			out << out_scale << endl;
			//out << "# Threshold to make leaf vote for particular class center" << endl;
			out << prob_threshold << endl;
			//out << "# Width for classes" << endl;
			for (int i=0;i<class_count; i++)
				out << width_aver[i]<<" ";
			//out <<endl << "# height_min"<< endl;
			out << endl;
			for (int i=0;i<class_count; i++)
				out << height_min[i]<<" ";
		}
	}

	void load(const char* filename)
	{
		FILE * pFile = fopen (filename,"r");
		if (pFile != NULL)
		{
			fscanf (pFile, "%i", &class_count);
			fscanf (pFile, "%i %i", &width, &height);
			fscanf (pFile, "%i", &out_scale);
			fscanf (pFile, "%f", &prob_threshold);
			width_aver.resize(class_count);
			height_min.resize(class_count);
			for (int i=0;i<class_count; i++)
				fscanf (pFile, "%i", &width_aver[i]);
			for (int i=0;i<class_count; i++)
				fscanf (pFile, "%i", &height_min[i]);
		}
	}

private:
	void detectColor(cv::Mat img, vector<cv::Mat>& imgDetect, vector<vector<list<float>>>& ratios);
	const CRForest* crForest;
	int width;
	int height;
	float prob_threshold; // leaf must have (>=prob_threshold) to be able to vote for center
	vector<int> width_aver;
	vector<int> height_min;
	int class_count;
	int out_scale;
};
