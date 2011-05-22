#ifndef CHMODSRV_HH
#define CHMODSRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getChangeModeProfile();
int solveChangeMode(diet_profile_t* profile);

#endif
