// Sample class test for the FMS module

#ifndef __FMS__
#define __FMS__

#include "FMSVishnuException.hpp"
#include "VishnuException.hpp"

class FMS{
private :
  void copy();
  void remove();
  void chmod();
  void chown();
  void advancement();
  FMSVishnuException e;
public :
  FMS();
  ~FMS(){};
  void call (int val);
};

#endif // FMS
