/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#define _copysign copysign

#include "HoG.h"

// structure for image patch
struct PatchFeature {
public:
	PatchFeature() {}

	cv::Size size;
	cv::Point center;

	std::vector<cv::Mat> vPatch;
	void show(int delay) const;
};

static HoG hog; 

class CRPatch {
public:
	// num_l - number of classes
	CRPatch(CvRNG* pRNG, int w, int h, int num_l) : cvRNG(pRNG), width(w), height(h) { vLPatches.resize(num_l+1);}

	// Extract patches from image
	// label - label of class
	void extractPatches(cv::Mat img, unsigned int n, int label, cv::Rect* box = 0, cv::Point* vCenter = 0);

	// Extract features from image
	static void extractFeatureChannels(cv::Mat img, std::vector<cv::Mat>& vImg);
	// min/max filter
	static void maxfilt(uchar* data, uchar* maxvalues, unsigned int step, unsigned int size, unsigned int width);
	static void maxfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width);
	static void minfilt(uchar* data, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width);
	static void minfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width);
	static void maxminfilt(uchar* data, uchar* maxvalues, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width);
	static void maxfilt(cv::Mat src, unsigned int width);
	static void maxfilt(cv::Mat src, cv::Mat dst, unsigned int width);
	static void minfilt(cv::Mat src, unsigned int width);
	static void minfilt(cv::Mat src, cv::Mat dst, unsigned int width);

	std::vector<std::vector<PatchFeature> > vLPatches; // outside vector is used for classes. [0 .. num_of_classes-1] - for classes, [num_of_classes] - background

	static float copysign(float x, float y)
	{
		if (y < 0)
			return -abs(x);
		return abs(x);
	}
private:
	CvRNG *cvRNG;
	int width;
	int height;
};

