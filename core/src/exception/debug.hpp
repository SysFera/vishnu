/**
 * \file debug.hpp
 * \brief This file defines the debug functions
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#include <errno.h>

#define ERRMSG(msg) fprintf(stderr, "Error line %d in file %s with message :\n %s \n", __LINE__, __FILE__, msg);

