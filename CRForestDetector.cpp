/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/
#pragma once
#include "stdafx.h"
#include "CRForestDetector.h"

using namespace std;

// imgDetect - vector.size == num_of_classes
int CRForestDetector::detectColor(cv::Mat img, vector<cv::Mat>& imgDetect, vector<cv::Mat>& ratios) {

	int img_width = img.rows;
	// extract features
	std::vector<cv::Mat> vImg;
	CRPatch::extractFeatureChannels(img, vImg);

	int timer = clock();
	// reset output image
	for(int c=0; c < num_of_classes; ++c)
	{
		imgDetect[c] = cv::Mat::zeros(imgDetect[c].rows, imgDetect[c].cols, CV_32FC1); // CV_32FC1 !!
		ratios[c] = cv::Mat::zeros(imgDetect[c].rows, imgDetect[c].cols, CV_32FC2);
	}

	// get pointers to feature channels
	int stepImg;
	uchar** ptFCh     = new uchar*[vImg.size()];
	uchar** ptFCh_row = new uchar*[vImg.size()];
	for(unsigned int c=0; c<vImg.size(); ++c) {
		ptFCh[c] = vImg[c].data;
	}
	stepImg = vImg[0].step1();

	// get pointer to output image
	int stepDet;
	int stepRatio;
	float** ptDet = new float*[num_of_classes];
	float** ptRatio = new float*[num_of_classes];
	for(unsigned int c=0; c < num_of_classes; ++c)
	{
		ptDet[c] = (float*)imgDetect[c].data;
		ptRatio[c] = (float*)ratios[c].data;
	}
	stepDet = imgDetect[0].step1();
	stepRatio = ratios[0].step1();

	int xoffset = width/2;
	int yoffset = height/2;
	
	int x, y, cx, cy; // x,y top left; cx,cy center of patch
	cy = yoffset; 

	for(y=0; y<img.rows-height; ++y, ++cy) 
	{
		// Get start of row
		for(unsigned int c=0; c<vImg.size(); ++c)
			ptFCh_row[c] = &ptFCh[c][0];
		cx = xoffset; 
		
		for(x=0; x<img.cols-width; ++x, ++cx) 
		{					
			// regression for a single patch
			vector<const LeafNode*> result;
			crForest->regression(result, ptFCh_row, stepImg);
			
			// vote for all trees (leafs) 
			for(vector<const LeafNode*>::const_iterator itL = result.begin(); itL!=result.end(); ++itL)
			{

				for (int c = 0; c < num_of_classes; c++)
				{

				// To speed up the voting, one can vote only for patches 
			        // with a probability for foreground > 0.5
			        // 
				/*if ((*itL)->pfg[c] > prob_threshold)
					{*/
						// voting weight for leaf 
						float w = (*itL)->pfg[c] / float( (*itL)->vCenter[c].size() * result.size() );
						float r = (*itL)->vRatio[c];

						// vote for all points stored in the leaf
						for(vector<cv::Point>::const_iterator it = (*itL)->vCenter[c].begin(); it!=(*itL)->vCenter[c].end(); ++it)
						{
							int x = int(cx - (*it).x + 0.5);
							int y = cy-(*it).y;
							if(y >= 0 && y < imgDetect[c].rows && x >= 0 && x<imgDetect[c].cols)
							{
								*(ptDet[c]+x+y*stepDet) += w;
								//formula for pointer: *(ptM[mat_num] + row*step + col*channels_total + channel)
								*(ptRatio[c] + y*stepRatio + x*2)+=r;
								*(ptRatio[c] + y*stepRatio + x*2 + 1)+=1;
							}
						}
					//} // end if
				}
			}

			// increase pointer - x
			for(unsigned int c=0; c<vImg.size(); ++c)
				++ptFCh_row[c];

		} // end for x

		// increase pointer - y
		for(unsigned int c=0; c<vImg.size(); ++c)
			ptFCh[c] += stepImg;

	} // end for y 	

	timer = clock() - timer;
	// smooth result image
	for(int c=0; c<(int)imgDetect.size(); ++c)
		cv::GaussianBlur(imgDetect[c], imgDetect[c], cv::Size(3,3), 0);

	// release feature channels
	for(unsigned int c=0; c<vImg.size(); ++c)
		vImg[c].release();
	
	delete[] ptFCh;
	delete[] ptFCh_row;
	delete[] ptDet;
	delete[] ptRatio;
	return timer;
}

inline bool hides(cv::Rect r1, cv::Rect r2, float persent)
{
	cv::Rect inters = r1&r2;
	int a_r1 = r1.width*r1.height;
	int a_r2 = r2.width*r2.height;
	int a_int = inters.width*inters.height;
	return (a_r1 < a_r2) ? (float)a_int/a_r1 > persent : (float)a_int/a_r2 > persent;
}

// img - input image
// scales - scales to detect objects with different sizes
// vImgDetect [0..scales], inside vector is for different classes
void CRForestDetector::detectPyramid(cv::Mat img, vector<float>& scales, vector<vector<cv::Mat>>& vImgDetect, Results& result)
{	
	// [0] - summary time
	// [1] - init. maps
	// [2] - detectColor voting only (GPU)
	// [3] - detectColor total (with voting)
	// [4] - localMax function
	// [5] - max. find other operations
	// [6] - convert to Results
	result.time = vector<float>(7);
	int init_maps = 0, detecCol = 0, maxFind = 0, localmax = 0;
	if(img.channels() == 1) 
	{
		throw string_exception("Gray color images are not supported.");
	} 
	else 
	{
		result.time[0] = clock();

		vImgDetect.resize(scales.size());

		vector<MaxPoint> maxs;
		int max_index = 0;
		// run detector for all scales
		for(int i=0; i < scales.size(); i++) 
		{
			init_maps = clock();
			// mats for classes and [i] scale
			vector<cv::Mat> tmps(num_of_classes);
			vImgDetect[i].resize(num_of_classes);
			for(unsigned int c = 0; c < num_of_classes; c++)
			{
				tmps[c].create ( cvSize(int(img.cols*scales[i]+0.5),int(img.rows*scales[i]+0.5)), CV_32FC1 );
			}

			cv::Mat cLevel (tmps[0].rows, tmps[0].cols, CV_8UC3);				
			cv::resize( img, cLevel, cv::Size(tmps[0].cols, tmps[0].rows));//CV_INTER_LINEAR is default
			
			vector<cv::Mat> ratios(num_of_classes);

			init_maps = clock() - init_maps;
			result.time[1] += init_maps;

			// detection
			detecCol = clock();
			result.time[2] += detectColor(cLevel, tmps, ratios);
			detecCol = clock() - detecCol;
			result.time[3] += detecCol;
			
			for (int c = 0; c < num_of_classes; c++) {
				tmps[c].convertTo(vImgDetect[i][c], CV_8UC1, testParam.out_scale);
				tmps[c].release();
			}

			
			localmax = clock();
			for (int c = 0; c < num_of_classes; c++)
				localMaxima(vImgDetect[i][c], cv::Size(width_aver[c]*scales[i], height_min[c]*scales[i]), maxs, c, testParam.max_treshold);
			localmax = clock() - localmax;
			result.time[4] = localmax;

			maxFind = clock();
			for (int k = maxs.size()-1; k>=max_index;k--)
			{
				int cl = maxs[k].class_label;
				cv::Vec2f vec = ratios[cl].at<cv::Vec2f>(maxs[k].point.y, maxs[k].point.x);
				maxs[k].ratio = vec[0]/(float)vec[1];
				maxs[k].point.x /= scales[i];
				maxs[k].point.y /= scales[i];
				maxs[k].scale = scales[i];
			}
			max_index = maxs.size();

			maxFind = clock()-maxFind;
			result.time[5]+=maxFind;

			for (int z = 0; z < ratios.size(); z++)
				ratios[z].release();
			ratios.clear();
				
			cLevel.release();
		}

		// convert to Results
		result.time[6] = clock();
		ConvertToTesults(maxs, result);
		result.time[6] = clock() - result.time[6];

		maxs.clear();
		//timers[0] = (double)(clock() - timers[0])/CLOCKS_PER_SEC;
		result.time[0] = clock() - result.time[0];

		for (int i = 0; i<result.time.size(); i++)
			result.time[i] /= CLOCKS_PER_SEC;
	}
}

void CRForestDetector::ConvertToTesults(vector<MaxPoint>& maxs,Results& result)
{
	for (int i = maxs.size()-1; i > 0; i--)
		{
			int cl = maxs[i].class_label;
			int w = width_aver[cl]/maxs[i].scale;
			int h = w*maxs[i].ratio;
			cv::Rect rect(maxs[i].point.x-w/2, maxs[i].point.y-h/2, w,h);
			for (int j = 0; j < i;j++)
			{
				int cl2 = maxs[j].class_label;
				int w2 = width_aver[cl2]/maxs[j].scale;
				int h2 = w2*maxs[j].ratio;
				cv::Rect rect2(maxs[j].point.x-w2/2, maxs[j].point.y-h2/2, w2,h2);

				if (hides(rect, rect2, 0.75f) )
				{
					if (maxs[j].pf > maxs[i].pf)
					{
						maxs[i].pf = 0;
						break;
					}
					else
					{
						maxs[j].pf = 0;
					}
				}
			}
		}
		for (int j = 0; j < maxs.size(); j++)
		{
			if (maxs[j].pf != 0)
			{
				result.classes.push_back(maxs[j].class_label); 
				int w = width_aver[maxs[j].class_label]/maxs[j].scale;
				int h = w * maxs[j].ratio;
				result.rects.push_back(cv::Rect(maxs[j].point.x - w/2, maxs[j].point.y - h/2, w, h));
			}
		}
}

int CRForestDetector::maxUsedValInHistogramData(cv::Mat src)
{
	HierarhicalThreshold ht(src);
	int bins = 10;
	if (ht.ProcessImage(1, bins))
		return 255/bins*ht.thresholds_for_max_sigma[0];
	return 0;
}

bool CRForestDetector::localMaxima(cv::Mat src, cv::Size size, std::vector<MaxPoint>& locations, int class_label, int threshold)
{
	cv::Mat m0;
	cv::Mat dst(src.rows, src.cols, CV_8UC1);
	cv::Point maxLoc(0,0);

	//1.Be sure to have at least 3x3 for at least looking at 1 pixel close neighbours
	//  Also the window must be <odd>x<odd>
	//SANITYCHECK(squareSize,3,1);
	cv::Point sqrCenter ((size.width-1)/2.0, (size.height-1)/2.0);
	if (size.height % 2 == 0)
		size.height--;
	if (size.width % 2 == 0)
		size.width--;

	int half_height = size.height/2;
	int half_width = size.width/2;
	//2.Create the localWindow mask to get things done faster
	//  When we find a local maxima we will multiply the subwindow with this MASK
	//  So that we will not search for those 0 values again and again
	cv::Mat localWindowMask = cv::Mat::zeros(size,CV_8U);//boolean
	localWindowMask.at<unsigned char>(sqrCenter)=1;

	//3.Find the threshold value to threshold the image
		//this function here returns the peak of histogram of picture
		//the picture is a thresholded picture it will have a lot of zero values in it
		//so that the second boolean variable says :
		//  (boolean) ? "return peak even if it is at 0" : "return peak discarding 0"
	if (threshold == 0) //black image, no max
		return false;
	cv::threshold(src, dst, threshold, 1, CV_THRESH_TOZERO); // 5th parameter 3 === THRESH_BINARY

	//put the src in the middle of the big array
	for (int row=sqrCenter.y;row<dst.size().height-sqrCenter.y;row++)
	{
		for (int col=sqrCenter.x;col<dst.size().width-sqrCenter.x;col++)
			{
			//1.if the value is zero it can not be a local maxima
			if (dst.at<unsigned char>(row,col)==0)
				continue;
			//2.the value at (row,col) is not 0 so it can be a local maxima point
			m0 =  dst.colRange(col-sqrCenter.x,col+sqrCenter.x+1).rowRange(row-sqrCenter.y,row+sqrCenter.y+1);
			minMaxLoc(m0,NULL,NULL,NULL,&maxLoc);
			//if the maximum location of this subWindow is at center
			//it means we found the local maxima
			//so we should delete the surrounding values which lies in the subWindow area
			//hence we will not try to find if a point is at localMaxima when already found a neighbour was
			if ((maxLoc.x==sqrCenter.x)&&(maxLoc.y==sqrCenter.y))
			{
				bool skip = false;
				// check probability of other classes in area around found point
				for (vector<MaxPoint>::iterator mp = locations.begin(); mp != locations.end(); mp++)
				{
					if (cv::Rect(mp->point.x - half_width, mp->point.y - half_height, size.width, size.height).contains(cv::Point(col,row)))
					{
						if (mp->pf < dst.at<uchar>(row, col)) // if probability of previously detected max is less, replace
						{
							mp->class_label = class_label;
							mp->pf = dst.at<uchar>(row, col);
							mp->point.x = col;
							mp->point.y = row;
						}
						skip = true;
						break;
					}
				}
				if (!skip)
					locations.push_back(MaxPoint(col, row, dst.at<uchar>(row, col), class_label));
				m0 = m0.mul(localWindowMask);
								//we can skip the values that we already made 0 by the above function
				col+=sqrCenter.x;
			}
		}
	}
	return true;
}