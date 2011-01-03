// Sample class test for the TMS module

#ifndef __TMS__
#define __TMS__

#include "TMSVishnuException.hh"

class TMS{
private :
  void submitJob();
  void getJob();
  void cancelJob();
  void listQueue();
  void listJobs();
  TMSVishnuException* e;
public :
  TMS();
  ~TMS(){};
  void call (int val);
};

#endif // TMS
