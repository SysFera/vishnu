// Sample class test for the UMS module

#ifndef __UMS__
#define __UMS__

#include "UMSVishnuException.hh"
#include "VishnuException.hh"

class UMS{
private :
  void connect();
  void disconnect();
  void displaySession();
  void addMachine();
  void removeMachine();
  void saveConfiguration();
  void displayLog();
  UMSVishnuException* e;
public :
  UMS();
  ~UMS(){};
  void call (int val);
};

#endif // UMS
