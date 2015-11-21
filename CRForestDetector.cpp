/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/
#pragma once
#include "stdafx.h"
#include "CRForestDetector.h"
//#include <vector>
#include <time.h>

using namespace std;


void CRForestDetector::detectColor(cv::Mat img, std::vector<cv::Mat>& imgDetect, std::vector<float>& ratios) {

	// extract features
	std::vector<cv::Mat> vImg;
	CRPatch::extractFeatureChannels(img, vImg);

	// reset output image
	for(int c=0; c<(int)imgDetect.size(); ++c)
		imgDetect[c] = cv::Mat::zeros(imgDetect[c].rows, imgDetect[c].cols, CV_32FC1); // CV_32FC1 !!
		//cvSetZero( &imgDetect[c] );

	// get pointers to feature channels
	int stepImg;
	uchar** ptFCh     = new uchar*[vImg.size()];
	uchar** ptFCh_row = new uchar*[vImg.size()];
	for(unsigned int c=0; c<vImg.size(); ++c) {
		ptFCh[c] = vImg[c].data;
		//cvGetRawData( &vImg[c], (uchar**)&(ptFCh[c]), &stepImg);
	}
	stepImg = vImg[0].step1();

	// get pointer to output image
	int stepDet;
	float** ptDet = new float*[imgDetect.size()];
	for(unsigned int c=0; c<imgDetect.size(); ++c)
	{
		ptDet[c] = (float*)imgDetect[c].data;
		//cvGetRawData( &imgDetect[c], (uchar**)&(ptDet[c]), &stepDet);
	}
	stepDet = imgDetect[0].step1();

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

				for (vector<float>::const_iterator pfgi = pfg.begin(); pfgi!=pfg.end(); ++pfgi)
				{
				// To speed up the voting, one can vote only for patches 
			        // with a probability for foreground > 0.5
			        // 
				// if((*itL)->pfg>0.5) {

					// voting weight for leaf 
					float w = (*itL)->pfg / float( (*itL)->vCenter.size() * result.size() );

					// vote for all points stored in the leaf
					for(vector<vector<cv::Point> >::const_iterator it = (*itL)->vCenter.begin(); it!=(*itL)->vCenter.end(); ++it)
					{

						for(int c=0; c<(int)imgDetect.size(); ++c) 
						{
							  int x = int(cx - (*it)[0].x * ratios[c] + 0.5);
							  int y = cy-(*it)[0].y;
							  if(y>=0 && y<imgDetect[c].rows && x>=0 && x<imgDetect[c].cols)
							  {
									*(ptDet[c]+x+y*stepDet) += w;
							  }
						}
					}

				 // } // end if
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

	// smooth result image
	for(int c=0; c<(int)imgDetect.size(); ++c)
		cv::GaussianBlur(imgDetect[c], imgDetect[c], cv::Size(3,3), 0);
		//cvSmooth( imgDetect[c], imgDetect[c], CV_GAUSSIAN, 3);

	// release feature channels
	for(unsigned int c=0; c<vImg.size(); ++c)
		vImg[c].release();
	
	delete[] ptFCh;
	delete[] ptFCh_row;
	delete[] ptDet;

}

void CRForestDetector::detectPyramid(cv::Mat img, vector<vector<cv::Mat>>& vImgDetect, std::vector<float>& ratios) {	

	if(img.channels() == 1) 
	{
		throw string_exception("Gray color images are not supported.");
	} 
	else 
	{ // color

		cout << "Timer" << endl;
		int tstart = clock();

		for(int i=0; i<int(vImgDetect.size()); ++i) {
			cv::Mat cLevel (vImgDetect[i][0].rows, vImgDetect[i][0].cols, CV_8UC3);				
			cv::resize( img, cLevel, cv::Size(vImgDetect[i][0].cols,vImgDetect[i][0].rows));//CV_INTER_LINEAR is default
			// detection
			detectColor(cLevel,vImgDetect[i],ratios);

			cLevel.release();
		}

		cout << "Time " << (double)(clock() - tstart)/CLOCKS_PER_SEC << " sec" << endl;

	}

}








