/**
 * \file FMSConstants.hpp
 * This file contains FMS useful constans
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */

#ifndef FFMSCONSTANTS_HPP
#define FFMSCONSTANTS_HPP




#include <sys/types.h>
#include <sys/stat.h>

#ifdef __WIN32__
#include "OSIndependance.hpp"
#endif

/**
 * \brief default access permissions used for new file
 */
static const mode_t defaultFileAccessMode=420;

/**
 * \brief default access permissions used for new directory
 */
static const mode_t defaultDirectoryAccessMode=493;



#endif
