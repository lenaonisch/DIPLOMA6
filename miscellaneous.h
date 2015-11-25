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
	Results(){}
};

struct MaxPoint {
public:
	float pf; // probability
	cv::Point point;
	int class_label;

	MaxPoint(){}
	MaxPoint(int x, int y, float _pf, int _class_label) : class_label(_class_label), point(x,y), pf(_pf){}
};