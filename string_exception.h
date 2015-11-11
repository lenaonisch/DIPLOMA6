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