#include <errno.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>

#include "UMSVishnuException.hh"


#define ERRMSG(msg) fprintf(stderr, "Error line %d in file %s with message :\n %s \n", __LINE__-2, __FILE__, msg)


void sendErrorMsg(std::string msg);

void checkErrorMsg(std::string msg);
