#pragma once
#include "stdafx.h"

class HierarhicalThreshold
{
private:
	cv::Mat src;
	cv::Mat P; //float[,]
    cv::Mat S; //float[,]
    cv::Mat H; // double[,]
	cv::Mat histogram; // float[]
	int L;
	double max_sigma;
	void Find_P_S_H_matrix();
	void Histogram(int L);
	void ProcessThresholds(int M, vector<int> found_thresholds);

	int find (vector<int> vec, const int val)
	{
		int n = vec.size();
		for (int i = 0; i < n; i++)
			if (vec[i] == val)
				return i;
		return -1;
	}
public:
	vector<int> thresholds_for_max_sigma;
	HierarhicalThreshold(cv::Mat src);
	bool ProcessImage(int M, int bins); // M - number of thresholds, bins - ךמכ-גמ סעמכבצמג ג דטסעמדנאללו
	~HierarhicalThreshold(void);
};

