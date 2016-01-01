/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/
#pragma once
#include "stdafx.h"
#include "CRPatch.h"

using namespace std;
using namespace cv;

void CRPatch::extractPatches(Mat img, unsigned int n, int label, cv::Point * center) {
	// extract features
	vector<cv::Mat> vImg; // uchar, CV_8UC1
	extractFeatureChannels(img, vImg);
	cv::Rect roi(0, 0, width, height); //area that will be cloned

	Mat tmp;
	int offx = width/2; 
	int offy = height/2;
	float ratio = (float)img.rows/img.cols;

	// generate x,y locations
	Mat locations ( n, 1, CV_32SC2 );
	cv::randu(locations, Scalar(0,0,0,0), Scalar(img.cols-width,img.rows-height,0,0));

	// reserve memory
	unsigned int offset = vLPatches[label].size();
	vLPatches[label].reserve(offset+n);
	for(unsigned int i=0; i<n; ++i) {
		Vec2i point = locations.at<Vec2i>(i,0);
		
		PatchFeature pf;

		roi.x = point[0];
		roi.y = point[1];

		if(center!=0) 
		{
			pf.center.x = point[0] + offx - (*center).x;
			pf.center.y = point[1] + offy - (*center).y;
			pf.ratio = ratio;
		}

		vLPatches[label].push_back(pf);

		vLPatches[label].back().vPatch.resize(vImg.size());
		for(unsigned int c=0; c<vImg.size(); ++c) {
			vLPatches[label].back().vPatch[c] = vImg[c](roi).clone();
		}
	}

	locations.release();
	for(unsigned int c=0; c<vImg.size(); ++c)
		vImg[c].release();
}

void CRPatch::extractFeatureChannels(cv::Mat img, std::vector<cv::Mat>& vImg) {
	// 32 feature channels
	// 7+9 channels: L, a, b, |I_x|, |I_y|, |I_xx|, |I_yy|, HOGlike features with 9 bins (weighted orientations 5x5 neighborhood)
	// 16+16 channels: minfilter + maxfilter on 5x5 neighborhood 

	vImg.resize(32);
	Mat m (cvSize(img.cols,img.rows), CV_8UC1); 
	for(unsigned int c=0; c<vImg.size(); ++c)
		vImg[c].push_back(m); 

	// Get intensity
	cvtColor( img, vImg[0], CV_RGB2GRAY );

	// Temporary images for computing I_x, I_y (Avoid overflow for cvSobel)
	Mat I_x;// (cvSize(img.cols,img.rows), CV_16SC1); 
	Mat I_y; //(cvSize(img.cols,img.rows), CV_16SC1);
	
	// |I_x|, |I_y|
	int scale = 1;
	int delta = 0;
	int depth = CV_16S;
	Sobel(vImg[0], I_x, depth, 1, 0, 3, scale, delta, BORDER_DEFAULT);	

	Sobel(vImg[0], I_y, depth, 0, 1, 3, scale, delta, BORDER_DEFAULT);			

	convertScaleAbs( I_x, vImg[3], 0.25);

	convertScaleAbs( I_y, vImg[4], 0.25);
	
	Size size(I_x.cols, I_x.rows);
  
	short* I_xptr = (short*)I_x.data;
	short* I_yptr = (short*)I_y.data;
	int stepx = I_x.step1();
	int stepy = I_y.step1();

	// Orientation of gradients
	uchar* ptrImg1 = vImg[1].data;
	int step1 = vImg[1].step1();
	for( int y = 0; y < size.height; y++  )
	{	
		for( int x = 0; x < size.width; x++ ) 
		{
			// Avoid division by zero
			float tmp = I_xptr[x];//(float)I_x.at<short>(y,x);
			float tx = tmp + copysign(0.000001f, tmp);
			// Scaling [-pi/2 pi/2] -> [0 80*pi]
			ptrImg1[x]=uchar( ( atan(I_yptr[x]/tx)+3.14159265f/2.0f ) * 80 ); 
		}
		I_xptr+=stepx; I_yptr+=stepy; ptrImg1+=step1;
	}
	
	I_xptr = (short*)I_x.data;
	I_yptr = (short*)I_y.data;
	// Magnitude of gradients
	uchar* ptrImg2 = vImg[2].data;
	int step2 = vImg[2].step1();
	for(int y = 0; y < size.height; y++  )
	{
		for(int x = 0; x < size.width; x++ ) 
		{
			float xval = I_xptr[x];
			float yval = I_yptr[x];
			ptrImg2[x] = (uchar)( sqrt(xval*xval + yval*yval) );
		}
		I_xptr+=stepx; I_yptr+=stepy; ptrImg2+=step2;
	}

	// 9-bin HOG feature stored at vImg[7] - vImg[15] 
	hog.extractOBin(vImg[1], vImg[2], vImg, 7);
	
	// |I_xx|, |I_yy|

	Sobel(vImg[0], I_x, depth, 2, 0, 3); //when ddepth=-1, the destination image will have the same depth as the source
	convertScaleAbs( I_x, vImg[5], 0.25);	
	
	Sobel(vImg[0], I_y, depth, 0, 2, 3);
	convertScaleAbs( I_y, vImg[6], 0.25);
	
	// L, a, b
	cvtColor( img, img, CV_RGB2Lab  );

	I_x.release();
	I_y.release();	
	
	std::vector<Mat> channels;
	channels.push_back(vImg[0]);
	channels.push_back(vImg[1]);
	channels.push_back(vImg[2]);
	cv::split( img, channels);

	// min filter
	for(int c=0; c<16; ++c)
		minfilt(vImg[c], vImg[c+16], 5);

	//max filter
	for(int c=0; c<16; ++c)
		maxfilt(vImg[c], 5);


	
#if 0
	// for debugging only
	char buffer[40];
	for(unsigned int i = 0; i<vImg.size();++i) {
		sprintf_s(buffer,"out-%d.png",i);
		cvNamedWindow(buffer,1);
		cvShowImage(buffer, vImg[i]);
		//cvSaveImage( buffer, vImg[i] );
	}

	cvWaitKey();

	for(unsigned int i = 0; i<vImg.size();++i) {
		sprintf_s(buffer,"%d",i);
		cvDestroyWindow(buffer);
	}
#endif


}

void CRPatch::extractFeatureChannels(cv::Mat img, cv::Mat& cImg)
{
	std::vector<cv::Mat> vImg;
	extractFeatureChannels(img, vImg);
	cv::merge(vImg, cImg);
}

void CRPatch::maxfilt(cv::Mat src, unsigned int width) {

	uchar* s_data = src.data;
	int step = src.step1();
	Size size(src.cols,src.rows);

	for(int  y = 0; y < size.height; y++) {
		maxfilt(s_data+y*step, 1, size.width, width);
	}

	s_data = src.data;

	for(int  x = 0; x < size.width; x++)
		maxfilt(s_data+x, step, size.height, width);

}

void CRPatch::maxfilt(cv::Mat src, cv::Mat dst, unsigned int width) {

	uchar* s_data = src.data;
	uchar* d_data = dst.data;
	int step = dst.step1();
	Size size(dst.cols, dst.rows);

	for(int  y = 0; y < size.height; y++)
		maxfilt(s_data+y*step, d_data+y*step, 1, size.width, width);

	d_data = src.data;

	for(int  x = 0; x < size.width; x++)
		maxfilt(d_data+x, step, size.height, width);

}

void CRPatch::minfilt(cv::Mat src, unsigned int width) {

	uchar* s_data = src.data;
	int step = src.step1();
	Size size(src.cols, src.rows);

	for(int  y = 0; y < size.height; y++)
		minfilt(s_data+y*step, 1, size.width, width);

	s_data = src.data;

	for(int  x = 0; x < size.width; x++)
		minfilt(s_data+x, step, size.height, width);

}

void CRPatch::minfilt(cv::Mat src, cv::Mat dst, unsigned int width) {

	uchar* s_data = src.data;
	uchar* d_data = dst.data;
	int step = dst.step1();
	Size size(dst.cols,dst.rows);

	for(int  y = 0; y < size.height; y++)
		minfilt(s_data+y*step, d_data+y*step, 1, size.width, width);

	d_data = src.data;

	for(int  x = 0; x < size.width; x++)
		minfilt(d_data+x, step, size.height, width);

}


void CRPatch::maxfilt(uchar* data, uchar* maxvalues, unsigned int step, unsigned int size, unsigned int width) {

	unsigned int d = int((width+1)/2)*step; 
	size *= step;
	width *= step;

	maxvalues[0] = data[0];
	for(unsigned int i=0; i < d-step; i+=step) {
		for(unsigned int k=i; k<d+i; k+=step) {
			if(data[k]>maxvalues[i]) maxvalues[i] = data[k];
		}
		maxvalues[i+step] = maxvalues[i];
	}

	maxvalues[size-step] = data[size-step];
	for(unsigned int i=size-step; i > size-d; i-=step) {
		for(unsigned int k=i; k>i-d; k-=step) {
			if(data[k]>maxvalues[i]) maxvalues[i] = data[k];
		}
		maxvalues[i-step] = maxvalues[i];
	}

    deque<int> maxfifo;
    for(unsigned int i = step; i < size; i+=step) {
		if(i >= width) {
			maxvalues[i-d] = data[maxfifo.size()>0 ? maxfifo.front(): i-step];
		}
    
		if(data[i] < data[i-step]) { 

			maxfifo.push_back(i-step);
			if(i==  width+maxfifo.front()) 
				maxfifo.pop_front();

		} else {

			while(maxfifo.size() > 0) {
				if(data[i] <= data[maxfifo.back()]) {
					if(i==  width+maxfifo.front()) 
						maxfifo.pop_front();
				break;
				}
				maxfifo.pop_back();
			}

		}

    }  

    maxvalues[size-d] = data[maxfifo.size()>0 ? maxfifo.front():size-step];
 
}

void CRPatch::maxfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width) {

	unsigned int d = int((width+1)/2)*step; 
	size *= step;
	width *= step;

	deque<uchar> tmp;

	tmp.push_back(data[0]);
	for(unsigned int k=step; k<d; k+=step) {
		if(data[k]>tmp.back()) tmp.back() = data[k];
	}

	for(unsigned int i=step; i < d-step; i+=step) {
		tmp.push_back(tmp.back());
		if(data[i+d-step]>tmp.back()) tmp.back() = data[i+d-step];
	}


    deque<int> minfifo;
    for(unsigned int i = step; i < size; i+=step) {
		if(i >= width) {
			tmp.push_back(data[minfifo.size()>0 ? minfifo.front(): i-step]);
			data[i-width] = tmp.front();
			tmp.pop_front();
		}
    
		if(data[i] < data[i-step]) { 

			minfifo.push_back(i-step);
			if(i==  width+minfifo.front()) 
				minfifo.pop_front();

		} else {

			while(minfifo.size() > 0) {
				if(data[i] <= data[minfifo.back()]) {
					if(i==  width+minfifo.front()) 
						minfifo.pop_front();
				break;
				}
				minfifo.pop_back();
			}

		}

    }  

	tmp.push_back(data[minfifo.size()>0 ? minfifo.front():size-step]);
	
	for(unsigned int k=size-step-step; k>=size-d; k-=step) {
		if(data[k]>data[size-step]) data[size-step] = data[k];
	}

	for(unsigned int i=size-step-step; i >= size-d; i-=step) {
		data[i] = data[i+step];
		if(data[i-d+step]>data[i]) data[i] = data[i-d+step];
	}

	for(unsigned int i=size-width; i<=size-d; i+=step) {
		data[i] = tmp.front();
		tmp.pop_front();
	}
 
}

void CRPatch::minfilt(uchar* data, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width) {

	unsigned int d = int((width+1)/2)*step; 
	size *= step;
	width *= step;

	minvalues[0] = data[0];
	for(unsigned int i=0; i < d-step; i+=step) {
		for(unsigned int k=i; k<d+i; k+=step) {
			if(data[k]<minvalues[i]) minvalues[i] = data[k];
		}
		minvalues[i+step] = minvalues[i];
	}

	minvalues[size-step] = data[size-step];
	for(unsigned int i=size-step; i > size-d; i-=step) {
		for(unsigned int k=i; k>i-d; k-=step) {
			if(data[k]<minvalues[i]) minvalues[i] = data[k];
		}
		minvalues[i-step] = minvalues[i];
	}

    deque<int> minfifo;
    for(unsigned int i = step; i < size; i+=step) {
		if(i >= width) {
			minvalues[i-d] = data[minfifo.size()>0 ? minfifo.front(): i-step];
		}
    
		if(data[i] > data[i-step]) { 

			minfifo.push_back(i-step);
			if(i==  width+minfifo.front()) 
				minfifo.pop_front();

		} else {

			while(minfifo.size() > 0) {
				if(data[i] >= data[minfifo.back()]) {
					if(i==  width+minfifo.front()) 
						minfifo.pop_front();
				break;
				}
				minfifo.pop_back();
			}

		}

    }  

    minvalues[size-d] = data[minfifo.size()>0 ? minfifo.front():size-step];
 
}

void CRPatch::minfilt(uchar* data, unsigned int step, unsigned int size, unsigned int width) {

	unsigned int d = int((width+1)/2)*step; 
	size *= step;
	width *= step;

	deque<uchar> tmp;

	tmp.push_back(data[0]);
	for(unsigned int k=step; k<d; k+=step) {
		if(data[k]<tmp.back()) tmp.back() = data[k];
	}

	for(unsigned int i=step; i < d-step; i+=step) {
		tmp.push_back(tmp.back());
		if(data[i+d-step]<tmp.back()) tmp.back() = data[i+d-step];
	}


    deque<int> minfifo;
    for(unsigned int i = step; i < size; i+=step) {
		if(i >= width) {
			tmp.push_back(data[minfifo.size()>0 ? minfifo.front(): i-step]);
			data[i-width] = tmp.front();
			tmp.pop_front();
		}
    
		if(data[i] > data[i-step]) { 

			minfifo.push_back(i-step);
			if(i==  width+minfifo.front()) 
				minfifo.pop_front();

		} else {

			while(minfifo.size() > 0) {
				if(data[i] >= data[minfifo.back()]) {
					if(i==  width+minfifo.front()) 
						minfifo.pop_front();
				break;
				}
				minfifo.pop_back();
			}

		}

    }  

	tmp.push_back(data[minfifo.size()>0 ? minfifo.front():size-step]);
	
	for(unsigned int k=size-step-step; k>=size-d; k-=step) {
		if(data[k]<data[size-step]) data[size-step] = data[k];
	}

	for(unsigned int i=size-step-step; i >= size-d; i-=step) {
		data[i] = data[i+step];
		if(data[i-d+step]<data[i]) data[i] = data[i-d+step];
	}
 
	for(unsigned int i=size-width; i<=size-d; i+=step) {
		data[i] = tmp.front();
		tmp.pop_front();
	}
}

void CRPatch::maxminfilt(uchar* data, uchar* maxvalues, uchar* minvalues, unsigned int step, unsigned int size, unsigned int width) {

	unsigned int d = int((width+1)/2)*step; 
	size *= step;
	width *= step;

	maxvalues[0] = data[0];
	minvalues[0] = data[0];
	for(unsigned int i=0; i < d-step; i+=step) {
		for(unsigned int k=i; k<d+i; k+=step) {
			if(data[k]>maxvalues[i]) maxvalues[i] = data[k];
			if(data[k]<minvalues[i]) minvalues[i] = data[k];
		}
		maxvalues[i+step] = maxvalues[i];
		minvalues[i+step] = minvalues[i];
	}

	maxvalues[size-step] = data[size-step];
	minvalues[size-step] = data[size-step];
	for(unsigned int i=size-step; i > size-d; i-=step) {
		for(unsigned int k=i; k>i-d; k-=step) {
			if(data[k]>maxvalues[i]) maxvalues[i] = data[k];
			if(data[k]<minvalues[i]) minvalues[i] = data[k];
		}
		maxvalues[i-step] = maxvalues[i];
		minvalues[i-step] = minvalues[i];
	}

    deque<int> maxfifo, minfifo;

    for(unsigned int i = step; i < size; i+=step) {
		if(i >= width) {
			maxvalues[i-d] = data[maxfifo.size()>0 ? maxfifo.front(): i-step];
			minvalues[i-d] = data[minfifo.size()>0 ? minfifo.front(): i-step];
		}
    
		if(data[i] > data[i-step]) { 

			minfifo.push_back(i-step);
			if(i==  width+minfifo.front()) 
				minfifo.pop_front();
			while(maxfifo.size() > 0) {
				if(data[i] <= data[maxfifo.back()]) {
					if (i==  width+maxfifo.front()) 
						maxfifo.pop_front();
					break;
				}
				maxfifo.pop_back();
			}

		} else {

			maxfifo.push_back(i-step);
			if (i==  width+maxfifo.front()) 
				maxfifo.pop_front();
			while(minfifo.size() > 0) {
				if(data[i] >= data[minfifo.back()]) {
					if(i==  width+minfifo.front()) 
						minfifo.pop_front();
				break;
				}
				minfifo.pop_back();
			}

		}

    }  

    maxvalues[size-d] = data[maxfifo.size()>0 ? maxfifo.front():size-step];
	minvalues[size-d] = data[minfifo.size()>0 ? minfifo.front():size-step];
 
}
