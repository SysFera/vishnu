// Sample class test for the IMS module

#ifndef __IMS__
#define __IMS__

#include "IMSVishnuException.hh"
#include "VishnuException.hh"

class IMS{
private :
  void getCPU();
  void getTMem();
  void getFMem();
  void getTD();
  void getFD();
  IMSVishnuException e;
public :
  IMS();
  ~IMS(){};
  void call (int val);
};

#endif // IMS
