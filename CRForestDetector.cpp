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
									index<2> tree_ID) restrict (amp, cpu)
{
	int nxt = leafpointer[index<2>(tree_ID[0], tree_ID[1]+1)];
	return leafs.section(tree_ID[0], leafpointer[tree_ID], 1, nxt - leafpointer[tree_ID]);
}

inline int regression_leaf_index(array_view<unsigned int, 1> vImgView, 
						  index<3>idx,
						  array_view<const int, 2> treetableView,
						  int channels, int step) restrict (amp, cpu)
{
	int x,y;
	int node_index = 0;
	auto treenode = treetableView.section(idx[0], node_index*7, 1, 7);
	while(treenode[index<2> (0, 0)] == -1)
	{
		// binary test 0 - left, 1 - right
		// Note that x, y are changed since the patches are given as matrix and not as image 
		// p1 - p2 < t -> left is equal to (p1 - p2 >= t) == false
		
		// get pixel values 
		int channel = treenode[index<2> (0, 5)];
		x = idx[2]+treenode[index<2> (0, 1)];
		y = idx[1]+treenode[index<2> (0, 2)];
		int p1 = read_uchar(vImgView, index<3>(y,x,channel), step, channels);
		x = idx[2]+treenode[index<2> (0, 3)];
		y = idx[1]+treenode[index<2> (0, 4)];
		int p2 = read_uchar(vImgView, index<3>(y,x,channel), step, channels);
		// test
		bool test = (p1 - p2) >= treenode[index<2> (0, 6)];

		// next node: 2*node_id + 1 + test
		// increment node/pointer by node_id + 1 + test
		int incr = node_index+1+test;
		node_index += incr; //after this operation node contains node_id
		treenode =  treetableView.section(idx[0], node_index*7, 1, 7);
	}
	return treenode[index<2> (0, 0)];
}

// imgDetect - vector.size == num_of_classes
int CRForestDetector::detectColor(cv::Mat img, cv::Size size, vector<cv::Mat>& imgDetect, vector<cv::Mat>& ratios) {

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
	imgDetect.resize(num_of_classes);
	ratios.resize(num_of_classes);
	for (int i = 0; i<num_of_classes; i++)
	{
		imgDetect[i] = cv::Mat::zeros(size, CV_32SC1); // CV_32FC1 !!
		ratios[i] = cv::Mat::zeros(size, CV_32SC1);
	}
	const int num_of_classes_ = num_of_classes;
	int xoffset = width/2;
	int yoffset = height/2;
	
	//int cx, cy; // x,y top left; cx,cy center of patch

	// treetable
	concurrency::extent<2> e_treetable(crForest->vTrees.size(), crForest->amp_treetable.cols);
	array_view<const int, 2> treetableView(e_treetable, (int*)crForest->amp_treetable.data);
	// leafs
	concurrency::extent<2> e_leafs(crForest->vTrees.size(), crForest->amp_leafs.cols);
	array_view<const int, 2> leafsView(e_leafs, (int*)crForest->amp_leafs.data);
	// leafpointer
	concurrency::extent<2> e_leafpointer(crForest->vTrees.size(), crForest->amp_leafpointer.cols);
	array_view<const int, 2> leafpointerView(e_leafpointer, (int*)crForest->amp_leafpointer.data);


	float prob_threshold = testParam.prob_threshold;

	int tree_count = crForest->vTrees.size();
	concurrency::extent<3> e_main(tree_count, img.rows-height, img.cols-width);
	for (int c = 0; c < num_of_classes_; c++)
	{
		// output image vImgDetect
		concurrency::extent<2> e_ptDet(size.height, size.width);
		array_view<int, 2> ptDetView(e_ptDet, (int*)imgDetect[c].data);
		//ptDetView.discard_data();

		// output matrices ratio
		concurrency::extent<2> e_ptRatio(size.height, size.width);
		array_view<unsigned int, 2> ptRatioView(e_ptRatio,  reinterpret_cast<unsigned int*>(ratios[c].data));
		//ptRatioView.discard_data();

		//accelerator_view av = accelerator(accelerator::direct3d_warp).create_view(queuing_mode_immediate); // this solution slows in 2 times!
		parallel_for_each(/*av, */e_main, [=](index<3>idx) restrict (amp)
		{
			int leaf_index = regression_leaf_index(vImgView, idx, treetableView, channels, step);
			auto leaf = GetLeafByID(leafsView, leafpointerView, index<2>(idx[0], leaf_index));		
			// To speed up the voting, one can vote only for patches 
				// with a probability for foreground > 0.5
				// 
			/*if (leaf[index<2>(0, 0)] > prob_threshold)
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

					while(cntr_size-- > 0)
					{	
						int x_ = int(idx[2] + xoffset - leaf[index<2>(0, ++ind)] + 0.5f);
						int y_ = idx[1] + yoffset - leaf[index<2>(0, ++ind)];
						if(y_ >= 0 && y_ < rows && x_ >= 0 && x_ < cols)
						{
							//atomic_fetch_add(&ptDetView[index<3>(y_, x_, c)], w);
							ptDetView[index<2>(y_, x_)] += w;
							int t = ptRatioView[index<2>(y_, x_)];
							// get first short: (t >> 16) & 0x0000FFFF
							//get second short: t & 0x0000FFFF
							// int combineddata = ((first<<16) | ((second) & 0xffff));
							ptRatioView[index<2>(y_, x_)] = (((((t >> 16) & 0xFFFF) + r) <<16) | (((t & 0xFFFF) + 1) & 0xFFFF));
						}
					}
				//} // end if
		});
		ptDetView.synchronize();
		ptRatioView.synchronize();
	}

	timer = clock() - timer;
	}
	catch(runtime_exception& ex)

{

  string sEx = ex.what();
  throw string_exception(sEx);

}

	// release feature channels
	for(unsigned int c=0; c<vImg_old.size(); ++c)
		vImg_old[c].release();
	vImg_old.clear();
	vCVMerge.release();
	
	return timer;
	//delete[] ptFCh;
	//delete[] ptFCh_row;
	//delete ptDet;
	//delete ptRatio;
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
			vector<cv::Mat> tmps;
			vImgDetect[i].resize(num_of_classes);
			cv::Size scaled_size(int(img.cols*scales[i]+0.5),int(img.rows*scales[i]+0.5));
			//tmps.create (scaled_size, CV_32FC(num_of_classes) );

			cv::Mat cLevel;// (scaled_size.height, scaled_size.width, CV_8UC3);				
			cv::resize( img, cLevel, scaled_size);//CV_INTER_LINEAR is default
			
			vector<cv::Mat> ratios;

			init_maps = clock() - init_maps;
			result.time[1] += init_maps;

			// detection
			detecCol = clock();
			result.time[2] += detectColor(cLevel, scaled_size, tmps, ratios);
			detecCol = clock() - detecCol;
			result.time[3] += detecCol;

			//cv::split(tmps, vImgDetect[i]);
			for (int c = 0; c<num_of_classes;c++)
			{
				tmps[c].convertTo(vImgDetect[i][c], CV_8UC1, 1/100.0f);
				GaussianBlur(vImgDetect[i][c], tmps[c], cv::Size(testParam.kernel, testParam.kernel), 0);
				tmps[c].convertTo(vImgDetect[i][c], CV_8UC1, testParam.out_scale/100.0f);
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
				unsigned int rat = ratios[cl].at<unsigned int>(maxs[k].point.y, maxs[k].point.x);
				ushort vec[] = {(rat >> 16) & 0x0000FFFF, rat & 0x0000FFFF};
				maxs[k].ratio = vec[0]/(float)(vec[1]*10);
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