#ifndef MACHINE_PROXY_H
#define MACHINE_PROXY_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "SessionProxy.h"
#include "UMS_Data/Machine.h"

class MachineProxy
{
private:
	UMS_Data::Machine machine;
	SessionProxy sessionProxy;
 
public:
	void MachineProxy(UMS_Data::Machine machine, SessionProxy session);
	int add();
	int update();
	int delete();
	void ~MachineProxy();
	SessionProxy getSessionProxy();
	UMS_Data::Machine getData();
};
#endif
