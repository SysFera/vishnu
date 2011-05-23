#ifndef MKDIRSRV_HH
#define MKDIRSRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getCreateDirProfile();
int solveCreateDir(diet_profile_t* profile);

#endif
