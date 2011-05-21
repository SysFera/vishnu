#ifndef SERVICES_HH
#define SERVICES_HH

#include <sys/types.h>

#include "File.hh"
using namespace FMS_Data;
/* Include all the services headers. */
#include "getInfosSrv.hh"
//#include "cpFileSrv.hh"
//#include "chgrpSrv.hh"
//#include "chmodSrv.hh"
#include "headSrv.hh"
#include "getContentSrv.hh"
#include "mkfileSrv.hh"
#include "mkdirSrv.hh"
#include "rmfileSrv.hh"
#include "rmdirSrv.hh"
#include "tailSrv.hh"
//#include "lsSrv.hh"
//#include "statusSrv.hh"

uid_t* uiddup(const uid_t& uid);
gid_t* giddup(const gid_t& gid);
mode_t* modedup(const mode_t& mode);
file_size_t* sizedup(const file_size_t& size);
time_t* timedup(const time_t& time);
FileType* typedup(const FileType& type);
#endif
