#pragma once
#include <StdAfx.h>
#include <exception>

using namespace std;
class string_exception: public exception
{
	string ex;
	string_exception(){	};
public:

	string_exception(string str)
	{
		ex.append(str);
	}
  virtual const char* what() const throw()
  {
	  return ex.c_str();
  }


};

struct Results
{	
public:
	//classes[i] corresponds to rects[i]
	QList<int> classes; // if defined - than multiclass detection. size must be the same as rects's size
	QList<cv::Rect> rects;
	QList<QColor> colors;
	bool processed;
	vector<float> time;

	Results():processed(false){}
	void clear()
	{
		classes.clear();
		rects.clear();
		colors.clear();
		time.swap(vector<float>());
		processed = false;
	}

	void removeAt(int res_index)
	{
		classes.removeAt(res_index);
		colors.removeAt(res_index);
		rects.removeAt(res_index);
	}

	void push_back(int class_, cv::Rect rect, QColor color)
	{
		classes.push_back(class_);
		rects.push_back(rect);
		colors.push_back(color);
	}

	cv::Rect makeNegative(int res_index)
	{
		classes[res_index] = num_of_classes;
		colors[res_index] = RED;
		return rects[res_index];
	}

	void markResWithColor(int res_index)
	{
		for (int i = 0; i < classes.size(); i++)
			if (classes[i] == num_of_classes)
				colors[i] = RED;
			else
				colors[i] = GREEN;
		colors[res_index] = VIOLET;
	}
};

struct MaxPoint {
public:
	float pf; // probability
	cv::Point point;
	int class_label;
	float ratio;
	float scale;

	MaxPoint(){}
	MaxPoint(int x, int y, float _pf, int _class_label) : class_label(_class_label), point(x,y), pf(_pf){}
};

struct TestParam
{
public:
	// test parameters
	int max_treshold; 
	int kernel;
	int out_scale; // scale factor for output image (default: 128)
	float prob_threshold; // leaf must have (>=prob_threshold) to be able to vote for center
	bool bSaveHoughSpace;
	~TestParam(){}
	TestParam():max_treshold(150), kernel(3), prob_threshold(0), out_scale(100), bSaveHoughSpace(false){}
	
	TestParam(int max_treshold, int kernel, int out_scale, float prob_threshold, bool bSaveHoughSpace):
		max_treshold(max_treshold), kernel(kernel), out_scale(out_scale), prob_threshold(prob_threshold), bSaveHoughSpace(bSaveHoughSpace){}
	
	TestParam& operator=(TestParam& right) { // copy/move constructor is called to construct arg
		if (this == &right) {
            return *this;
        }

		max_treshold = right.max_treshold;
		kernel = right.kernel;
		out_scale = right.out_scale;
		prob_threshold = right.prob_threshold;
		bSaveHoughSpace = right.bSaveHoughSpace;
		return *this;
	}
};