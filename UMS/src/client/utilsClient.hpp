#ifndef  _utilsClient_H_  
#define  _utilsClient_H_

#include <errno.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <cstring>
#include <sstream>
#include <algorithm>

#include "UMSVishnuException.hh"


#define ERRMSG(msg) fprintf(stderr, "Error line %d in file %s with message :\n %s \n", __LINE__-2, __FILE__, msg)


void sendErrorMsg(std::string msg);

void checkErrorMsg(std::string msg);

#endif
