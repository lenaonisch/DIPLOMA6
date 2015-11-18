#include "stdafx.h"
#include "HierarhicalThreshold.h"

void HierarhicalThreshold::Histogram(int L)
{
	int histSize = L;
	float range[] = { 0, 256 } ; //the upper boundary is exclusive
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	cv::calcHist( &src, 1, 0, cv::Mat(), histogram, 1, &histSize, &histRange, uniform, accumulate );
}
void HierarhicalThreshold::Find_P_S_H_matrix()
{
	P.create(L,L,CV_32F);
    S.create(L,L,CV_32F);
    H.create(L,L,CV_64F);

	P.at<float>(0, 0) = histogram.at<float>(0);
    S.at<float>(0, 0) = histogram.at<float>(0);
    H.at<double>(0, 0) = histogram.at<float>(0);
    for (int v = 1; v < L; v++)
    {
        P.at<float>(0, v) = P.at<float>(0, v - 1) + histogram.at<float>(v);
        S.at<float>(0, v) = S.at<float>(0, v - 1) + v * histogram.at<float>(v);
        H.at<double>(0, v) = pow(S.at<float>(0, v), 2) / P.at<float>(0, v);
    }

    for (int u = 1; u < L; u++)
    {
        for (int v = u; v < L; v++)
        {
            P.at<float>(u, v) = P.at<float>(0, v) - P.at<float>(0, u-1);
            S.at<float>(u, v) = S.at<float>(0, v) - S.at<float>(0, u-1);
            H.at<double>(u, v) = pow(S.at<float>(u, v), 2) / P.at<float>(u, v);
        }
    }
}
void HierarhicalThreshold::ProcessThresholds(int M, vector<int> found_thresholds)
{   
	int zero_thresh = find(found_thresholds, -1);
    if (zero_thresh > 0)//если Ќ≈ все пороги зафиксированы
    {
        for (int i = found_thresholds[zero_thresh - 1] + 1; i < L - M + zero_thresh; i++)
        {
            vector<int> to_process;
			for (int k = 0;k<found_thresholds.size();k++)
				to_process.push_back(found_thresholds[k]);
            to_process[zero_thresh] = i;
            ProcessThresholds(M, to_process);
        }
    }
    else //если все пороги зафиксированы считаем отклонение
    {
        double sigma = H.at<double>(0, found_thresholds[0]);
        for (int i = 1; i < M - 1; i++)
        {
			sigma += H.at<double>(found_thresholds[i - 1] + 1, found_thresholds[i]);
        }
        sigma += H.at<double>(found_thresholds[M - 1] + 1, L - 1);
        if (sigma > max_sigma)
        {
            max_sigma = sigma;
            thresholds_for_max_sigma = found_thresholds;
        }
    }
}

//M - кол-во порогов, которые надо найти
bool HierarhicalThreshold::ProcessImage(int M, int bins)
{
	L = bins;
	Histogram(L);
    Find_P_S_H_matrix();
    for (int i = 0; i < L - M ; i++) //фиксируем значение первого порога
    {
        vector<int> found_thresh;
        for (int k = 0; k < M; k++)
			found_thresh.push_back(-1);
        found_thresh[0] = i;
        ProcessThresholds(M, found_thresh);
    }

	if (thresholds_for_max_sigma.size()>0)
		for (int i = 0; i < thresholds_for_max_sigma.size(); i++)
			thresholds_for_max_sigma[i]++;
	else
		return false;
	return true;
}
HierarhicalThreshold::HierarhicalThreshold(cv::Mat src)
{
	max_sigma = 0;
	this->src = src;
}


HierarhicalThreshold::~HierarhicalThreshold(void)
{
}
