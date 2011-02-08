#ifndef __UTIL__

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "SessionProxy.h"
#include "MachineProxy.h"
#include "LocalAccountProxy.h"
#include "ConfigurationProxy.h"

void print(UMS_Data::User_ptr user, int id);
void print(UMS_Data::Machine_ptr machine, int id);
void  print(UMS_Data::LocalAccount_ptr localAccount, int id);

#endif
