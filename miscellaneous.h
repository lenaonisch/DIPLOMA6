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
	QList<int> classes; // if defined - than multiclass detection. size must be the same as rects's size
	QList<cv::Rect> rects;
};