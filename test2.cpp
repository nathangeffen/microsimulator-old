#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <tuple>
#include <random>

using namespace std;

class CustomisedException : public exception {
public:
  CustomisedException(string message, int line=0, string file="") {
    message_ = message;
    line_ = line;
    file_ = file;
    ::exception();
  }
  virtual const char* what() const throw()
  {
    stringstream message;

    message << message_ << " ";
    if ( line_ && file_ != "" )
      message << "Error at line " << line_ << " in " << file_;
    return message.str().c_str();
  }
  ~CustomisedException() throw () {};

private:
  string message_;
  int line_;
  string file_;
};

class myexception: public exception
{
  virtual const char* what() const throw()
  {
    return "My exception happened";
  }
};

int main(int argc, char *argv[])
{

  try
  {
    if (argc < 2) {
      throw CustomisedException("Too few arguments.", __LINE__, __FILE__);
    }
  }
  catch (CustomisedException& e)
  {
    cerr << "exception caught: " << e.what() << endl;
  }

  try
  {
    if (1 != 2) {
      throw myexception();
    }
  }
  catch (exception& e)
  {
    cerr << "exception caught: " << e.what() << endl;
  }

  try
  {
    if (1 != 2) {
      throw CustomisedException("Last one.");
    }
  }
  catch (exception& e)
  {
    cerr << "exception caught: " << e.what() << endl;
  }


  return 0;

}
