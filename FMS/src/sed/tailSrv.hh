#ifndef TAILSRV_HH
#define TAILSRV_HH

#include "DIET_server.h"

diet_profile_desc_t* getTailProfile();
int tailFile(diet_profile_t* profile);

#endif
