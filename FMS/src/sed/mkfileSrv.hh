#ifndef MKFILESRV_HH
#define MKFILESRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getCreateFileProfile();
int solveCreateFile(diet_profile_t* profile);

#endif
