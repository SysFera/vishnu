#ifndef CPSRV_HH
#define CPSRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getCopyFileProfile();
int solveCopyFile(diet_profile_t* profile);

diet_profile_desc_t* getCopyRemoteFileProfile();
int solveCopyRemoteFile(diet_profile_t* profile);
#endif
