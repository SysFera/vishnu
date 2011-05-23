#ifndef RMFILESRV_HH
#define RMFILESRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getRemoveFileProfile();
int solveRemoveFile(diet_profile_t* profile);

#endif
