// Sample class test for the IMS module

#ifndef __IMS__
#define __IMS__

#include "UserException.hpp"
#include "VishnuException.hpp"

class IMS{
private :
  void getCPU();
  void getTMem();
  void getFMem();
  void getTD();
  void getFD();
  UserException e;
public :
  IMS();
  ~IMS(){};
  void call (int val);
};

#endif // IMS
