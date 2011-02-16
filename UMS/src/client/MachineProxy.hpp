#ifndef _MACHINE_PROXY_H_
#define _MACHINE_PROXY_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.hpp"
#include "Machine.hpp"

class MachineProxy
{
 
public:
  MachineProxy(const UMS_Data::Machine& machine, const SessionProxy& session);
  int add();
  int update();
  int deleteMachine();
  SessionProxy getSessionProxy();
  UMS_Data::Machine getData();

  ~MachineProxy();

private:

  int _addMachineInformation(bool isNewMachine=true);

  UMS_Data::Machine mmachine;
  SessionProxy msessionProxy;

};
#endif
