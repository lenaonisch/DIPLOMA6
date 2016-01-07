/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/
#pragma once
#include "stdafx.h"
#include "CRForestDetector.h"

using namespace std;
using namespace concurrency;

// returns ONE-dimentional array with leaf data
inline array_view<const int,2> GetLeafByID(array_view<const int, 2> leafs, 
									array_view<const int,2> leafpointer, 
									index<2> tree_ID, int num_of_classes_) restrict (amp, cpu)
{
	int len = 2*num_of_classes_;
	//int* ptr = amp_leafs + tree * amp_leafs_step + (*(amp_leafpointer + tree*amp_leafpointer_step + ID));
	int leaf_index = leafpointer[tree_ID];
	len += leafs[index<2>(tree_ID[0], leaf_index + len)]*2 + 1;
	for (int i = 1; i < num_of_classes_; i++)
		len += leafs[index<2>(tree_ID[0], leaf_index + len)]*2 + 1;
	
	return leafs.section(tree_ID[0], leaf_index, 1, len);
}

inline int regression_leaf_index(array_view<unsigned int, 1> vImgView, 
						  index<3>idx,
						  array_view<const int, 2> treetableView,
						  array_view<const int, 2> treepointerView,
						  int channels, int step) restrict (amp, cpu)
{
	int tree_count = treetableView.extent[0];
	
	int node_index = 0;
	auto treenode = treetableView.section(idx[2], treepointerView[index<2>(idx[2], node_index)]*7, 1, 7);
	while(treenode[index<2> (0, 0)] == -1)
	{
		// binary test 0 - left, 1 - right
		// Note that x, y are changed since the patches are given as matrix and not as image 
		// p1 - p2 < t -> left is equal to (p1 - p2 >= t) == false
		
		// get pixel values 
		int channel = treenode[index<2> (0, 5)];
		int x1 = idx[1]+treenode[index<2> (0, 1)];
		int y1 = idx[0]+treenode[index<2> (0, 2)];
		int x2 = idx[1]+treenode[index<2> (0, 3)];
		int y2 = idx[0]+treenode[index<2> (0, 4)];
		int p1 = read_uchar(vImgView, index<3>(y1,x1,channel), step, channels);
		int p2 = read_uchar(vImgView, index<3>(y2,x2,channel), step, channels);
		// test
		bool test = (p1 - p2) >= treenode[index<2> (0, 6)];

		// next node: 2*node_id + 1 + test
		// increment node/pointer by node_id + 1 + test
		int incr = node_index+1+test;
		node_index += incr; //after this operation node contains node_id
		if (treepointerView[index<2>(idx[2], node_index)]<0)
			return 0;
		treenode =  treetableView.section(idx[2], treepointerView[index<2>(idx[2], node_index)]*7, 1, 7);
	}
	return treenode[index<2> (0, 0)];
}

// imgDetect - vector.size == num_of_classes
int CRForestDetector::detectColor(cv::Mat img, cv::Size size, cv::Mat& imgDetect, cv::Mat& ratios) {

	//// extract features
	//cv::Mat vImg;
	vector<cv::Mat> vImg_old;
	cv::Mat vCVMerge;
	////cv::Mat img_2;
	////img.copyTo(img_2);
	//CRPatch::extractFeatureChannels(img, vCVMerge);

	//using namespace concurrency;

	CRPatch::extractFeatureChannels(img, vImg_old);
	int timer = clock();
	int rows = size.height;
	int cols = size.width;
	//int sz[] = {rows,cols};
	int channels = vImg_old.size();
	vCVMerge.create(rows,cols, CV_8UC(channels)); 
	//int step = vCVMerge.step1();
try {
	//concurrency::extent<1> eOut((rows*cols*channels+3)/4);
	//array_view<unsigned int, 1> vImgView (eOut, reinterpret_cast<unsigned int*>(vCVMerge.data));
	
	int step = vCVMerge.step1();
	int step_input = vImg_old[0].step1();
	concurrency::extent<1> eOut((rows*cols*channels+3)/4);
	array_view<unsigned int, 1> vImgView (eOut, reinterpret_cast<unsigned int*>(vCVMerge.data));
	vImgView.discard_data();
	for(int c = 0; c < channels; c++)
	{
		concurrency::extent<1> eIn((rows*cols+3)/4);
		array_view<const unsigned int, 1> inputView (eIn, reinterpret_cast<unsigned int*>(vImg_old[c].data));	
		concurrency::extent<2> e(rows, cols);
		parallel_for_each(e, [=](index<2>idx) restrict (amp)
		{
			unsigned int ch = read_uchar(inputView, idx[0], idx[1], step_input);
			//write
			int index = idx[0]*step+idx[1]*channels+c;
			atomic_fetch_xor(&vImgView[index >> 2], vImgView[index >> 2] & (0xFF << ((index & 0x3) << 3)));
			atomic_fetch_xor(&vImgView[index >> 2], (ch & 0xFF) << ((index & 0x3) << 3));
		});
		vImgView.synchronize();
	}

	// reset output image
	imgDetect = cv::Mat::zeros(size, CV_32SC(num_of_classes)); // CV_32FC1 !!
	const int num_of_classes_ = num_of_classes;
	int numclass2 = num_of_classes*2;
	ratios = cv::Mat::zeros(size, CV_32SC(numclass2));

	int xoffset = width/2;
	int yoffset = height/2;
	
	//int cx, cy; // x,y top left; cx,cy center of patch

	// output image vImgDetect
	concurrency::extent<3> e_ptDet(size.height, size.width, num_of_classes);
	array_view<int, 3> ptDetView(e_ptDet, (int*)imgDetect.data);
	ptDetView.discard_data();

	// output matrices ratio
	concurrency::extent<3> e_ptRatio(size.height, size.width, numclass2);
	array_view<int, 3> ptRatioView(e_ptRatio, (int*)ratios.data);
	ptRatioView.discard_data();

	// treetable
	concurrency::extent<2> e_treetable(crForest->vTrees.size(), crForest->max_treetable_len*7);
	array_view<const int, 2> treetableView(e_treetable, (int*)crForest->amp_treetable.data);
	// treepointer
	concurrency::extent<2> e_treepointer(crForest->vTrees.size(), crForest->amp_treepointer.cols);
	array_view<const int, 2> treepointerView(e_treepointer, (int*)crForest->amp_treepointer.data);
	// leafs
	concurrency::extent<2> e_leafs(crForest->vTrees.size(), crForest->amp_leafs.cols);
	array_view<const int, 2> leafsView(e_leafs, (int*)crForest->amp_leafs.data);
	// leafpointer
	concurrency::extent<2> e_leafpointer(crForest->vTrees.size(), crForest->amp_leafpointer.cols);
	array_view<const int, 2> leafpointerView(e_leafpointer, (int*)crForest->amp_leafpointer.data);

	int tree_count = crForest->vTrees.size();
	concurrency::extent<3> e_main(img.rows-height, img.cols-width, tree_count);
	parallel_for_each(e_main, [=](index<3>idx) restrict (amp)
	{
		int leaf_index = regression_leaf_index(vImgView, idx, treetableView, treepointerView, channels, step);
		if (leaf_index != 0)
		{
			auto leaf = GetLeafByID(leafsView, leafpointerView, index<2>(idx[2], leaf_index), num_of_classes_);
			for (int c = 0; c < num_of_classes_; c++)
			{
			// To speed up the voting, one can vote only for patches 
				// with a probability for foreground > 0.5
				// 
			/*if ((*itL)->pfg[c] > prob_threshold)
				{*/
					// voting weight for leaf 
					int ind = 2*num_of_classes_; // indexation from zero, so formula isn't (2*num_of_classes + 1)
					int cntr_size = leaf[index<2>(0, ind)];
					for (int z = 1; z < num_of_classes_; z++)
					{
						ind += cntr_size*2+1;
						cntr_size = leaf[index<2>(0, ind)];
					}
					int w = leaf[index<2>(0, 2*c)]/float(cntr_size * tree_count);
					int r = leaf[index<2>(0, 2*c+1)];

					for (int point = 0; point < cntr_size; point++)
					{
						int x_ = int(idx[1] + xoffset - leaf[index<2>(0, ++ind)] + 0.5f);
						int y_ = idx[0] + yoffset - leaf[index<2>(0, ++ind)];
						if(y_ >= 0 && y_ < rows && x_ >= 0 && x_ < cols)
						{
							//atomic_fetch_add(&ptDetView[index<3>(y_, x_, c)], w);
							ptDetView[index<3>(y_, x_, c)] += w;
							ptRatioView[index<3>(y_, x_, 2*c)] += r;
							ptRatioView[index<3>(y_, x_, 2*c+1)]++;
						}
					}
				//} // end if
			}
		}	
	});
	
	ptDetView.synchronize();
	ptRatioView.synchronize();

	timer = clock() - timer;
	}
	catch(runtime_exception& ex)

{

  string sEx = ex.what();

}

	// release feature channels
	for(unsigned int c=0; c<vImg_old.size(); ++c)
		vImg_old[c].release();
	vCVMerge.release();
	
	return timer;
	//delete[] ptFCh;
	//delete[] ptFCh_row;
	//delete ptDet;
	//delete ptRatio;
}

// img - input image
// scales - scales to detect objects with different sizes
// vImgDetect [0..scales], inside vector is for different classes
double* CRForestDetector::detectPyramid(cv::Mat img, vector<float>& scales, vector<vector<cv::Mat>>& vImgDetect, Results& result)
{	
	int cl2 = num_of_classes*2;
	// [0] - summary time
	// [1] - init. maps
	// [2] - detectColor voting only (GPU)
	// [3] - detectColor total (with voting)
	// [4] - localMax function
	// [5] - max. find other operations
	// [6] - convert to Results
	double timers [10] = {0,0,0,0,0,0,0,0,0,0};
	double* t_ptr = &timers[0];
	int init_maps = 0, detecCol = 0, maxFind = 0, localmax = 0;
	if(img.channels() == 1) 
	{
		throw string_exception("Gray color images are not supported.");
	} 
	else 
	{
		timers[0] = clock();

		vImgDetect.resize(scales.size());

		vector<MaxPoint> maxs;
		int max_index = 0;
		// run detector for all scales
		for(int i=0; i < scales.size(); i++) 
		{
			init_maps = clock();
			// mats for classes and [i] scale
			cv::Mat tmps;
			vImgDetect[i].resize(num_of_classes);
			cv::Size scaled_size(int(img.cols*scales[i]+0.5),int(img.rows*scales[i]+0.5));
			//tmps.create (scaled_size, CV_32FC(num_of_classes) );

			cv::Mat cLevel (tmps.rows, tmps.cols, CV_8UC3);				
			cv::resize( img, cLevel, scaled_size);//CV_INTER_LINEAR is default
			
			cv::Mat ratios;

			init_maps = clock() - init_maps;
			timers[1] += init_maps;

			// detection
			detecCol = clock();
			timers[2] += detectColor(cLevel, scaled_size, tmps, ratios);
			detecCol = clock() - detecCol;
			timers[3] += detecCol;

			int treshold = 150;

			cv::split(tmps, vImgDetect[i]);
			for (int c = 0; c<num_of_classes;c++)
			{
				vImgDetect[i][c].convertTo(vImgDetect[i][c], CV_8UC1, out_scale/10000.0);
				GaussianBlur(vImgDetect[i][c], vImgDetect[i][c], cv::Size(3,3), 0);
			}
			tmps.release();

			
			localmax = clock();
			for (int c = 0; c < num_of_classes; c++)
				localMaxima(vImgDetect[i][c], cv::Size(width_aver[c]*scales[i], height_min[c]*scales[i]), maxs, c, treshold);
			localmax = clock() - localmax;
			timers[4] = localmax;

			maxFind = clock();
			int step = ratios.step1();
			for (int k = maxs.size()-1; k>=max_index;k--)
			{
				int cl = maxs[k].class_label;
				float* ptr = (float*)ratios.data + maxs[k].point.y*step + maxs[k].point.x * cl2 + 2*cl;
				float vec[] = {*ptr, *(ptr+1)};
				maxs[k].ratio = vec[0]/(float)(vec[1]*100);
				maxs[k].point.x /= scales[i];
				maxs[k].point.y /= scales[i];
				maxs[k].scale = scales[i];
			}
			max_index = maxs.size();

			maxFind = clock()-maxFind;
			timers[5]+=maxFind;

			ratios.release();
				
			cLevel.release();
		}

		// convert to Results
		timers[6] = clock();
		for (int i = maxs.size()-1; i > 0; i--)
		{
			int cl = maxs[i].class_label;
			int w = width_aver[cl];
			int h = w*maxs[i].ratio;
			cv::Rect rect(maxs[i].point.x-w/2, maxs[i].point.y-h/2, w,h);
			for (int j = 0; j < i;j++)
			{
				if (rect.contains(maxs[j].point) && maxs[j].pf > maxs[i].pf)
				{
					maxs[i].pf = 0;
					break;
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
		timers[6] = clock() - timers[6];

		maxs.clear();
		//timers[0] = (double)(clock() - timers[0])/CLOCKS_PER_SEC;
		timers[0] = clock() - timers[0];

		for (int i = 0; i<10; i++)
			timers[i] /= CLOCKS_PER_SEC;
	}
	result.time = timers[0];
	return t_ptr;
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

void CRForestDetector::convertToMultiChannel(array_view<unsigned int>& outputView, vector<cv::Mat> input)
{
	int sz[] = {input[0].rows, input[0].cols};
	int rows = sz[0];
	int cols = sz[1];
	int channels = input.size();
	int step = cols*channels;
	
	for(int c = 0; c < channels; c++)
	{
		using namespace concurrency;
	
		concurrency::extent<1> eIn((rows*cols+3)/4);
		array_view<const unsigned int, 1> inputView (eIn, reinterpret_cast<unsigned int*>(input[c].data));	

		concurrency::extent<2> e(rows, cols);
		parallel_for_each(e, [=](index<2>idx) restrict (amp)
		{
			unsigned int ch = read_uchar(inputView, idx, cols);
			//write
			int index = idx[0]*step+idx[1]*channels+c;
			atomic_fetch_xor(&outputView[index >> 2], outputView[index >> 2] & (0xFF << ((index & 0x3) << 3)));
			atomic_fetch_xor(&outputView[index >> 2], (ch & 0xFF) << ((index & 0x3) << 3));
		});
	}
	outputView.synchronize();
}