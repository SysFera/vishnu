
/**
 * \file internalApiFMS.hpp
 * This file contains the declaration of all vishnu FMS services headers
 */


#ifndef INTERNALAPI_HPP
#define INTERNALAPI_HPP

#include <sys/types.h>

#include "File.hpp"
using namespace FMS_Data;
/* Include all the services headers. */
#include "getInfosSrv.hpp"
#include "chgrpSrv.hpp"
#include "chmodSrv.hpp"
#include "headSrv.hpp"
#include "getContentSrv.hpp"
#include "mkfileSrv.hpp"
#include "mkdirSrv.hpp"
#include "rmfileSrv.hpp"
#include "rmdirSrv.hpp"
#include "tailSrv.hpp"
#include "lsSrv.hpp"
#include "transferSrv.hpp"

#endif
