#include <string>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/types.h>

#include "DIET_data.h"
#include "DIET_client.h"
#include "DIET_Dagda.h"

#include "RemoteFileProxy.hpp"
#include "LocalFileProxy.hpp"

using namespace std;
using namespace FMS_Data;

/* Default constructor. */
RemoteFileProxy::RemoteFileProxy() {
  upToDate = false;
}

/* Standard constructor.
 * Take the file path and user name as parameters.
 */
RemoteFileProxy::RemoteFileProxy(const SessionProxy& sessionProxy,const string& path,
                       const string& localUser) : FileProxy(sessionProxy,path)
{
  upToDate = false;
  this->localUser = localUser;
}

/* Copy constructor. */
RemoteFileProxy::RemoteFileProxy(const RemoteFileProxy& file) : FileProxy(file) {
  upToDate = false;
  this->localUser = file.localUser;
}

/* Standard destructor. */
RemoteFileProxy::~RemoteFileProxy() {
}

/* Returns true if the file informations are up to date. */
bool RemoteFileProxy::isUpToDate() const {
  return upToDate;
}

/* Get the informations about this remote file. Call the DIET service. */
void RemoteFileProxy::getInfos() const {
  diet_profile_t* profile;
  char* owner, * group, * errMsg;
  long* uid, * gid;
  long* perms, * size;
  long* atime, * mtime, * ctime;
  int* type;
  std::string serviceName("FileGetInfos");
  std::string sessionKey=this->getSession().getSessionKey();


  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 14);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);

  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 7), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 8), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 9), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 10), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 11), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 12), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 13), NULL, DIET_VOLATILE, DIET_INT);
  diet_string_set(diet_parameter(profile, 14), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to obtain file informations");
  diet_string_get(diet_parameter(profile, 4), &owner, NULL);
  diet_string_get(diet_parameter(profile, 5), &group, NULL);
  diet_scalar_get(diet_parameter(profile, 6), &uid, NULL);
  diet_scalar_get(diet_parameter(profile, 7), &gid, NULL);
  diet_scalar_get(diet_parameter(profile, 8), &perms, NULL);
  diet_scalar_get(diet_parameter(profile, 9), &size, NULL);
  diet_scalar_get(diet_parameter(profile, 10), &atime, NULL);
  diet_scalar_get(diet_parameter(profile, 11), &mtime, NULL);
  diet_scalar_get(diet_parameter(profile, 12), &ctime, NULL);
  diet_scalar_get(diet_parameter(profile, 13), &type, NULL);
  diet_string_get(diet_parameter(profile, 14), &errMsg, NULL);

  if (strlen(errMsg)!=0 || strlen(owner)==0) {
    exists(false);
    return;
  }
  setOwner(owner);
  setGroup(group);
  sysEndianChg<long>(*perms);
  setPerms(*perms);
  sysEndianChg<long>(*uid);
  setUid(*uid);
  sysEndianChg<long>(*gid);
  setGid(*gid);
  sysEndianChg<long>(*size);
  setSize(*size);
  sysEndianChg<long>(*atime);
  setAtime(*atime);
  sysEndianChg<long>(*mtime);
  setMtime(*mtime);
  sysEndianChg<long>(*ctime);
  setCtime(*ctime);
  sysEndianChg<int>(*type);
  setType((FileType) *type);
  exists(true);
  upToDate = true;
}

/* Copy operator. */
RemoteFileProxy& RemoteFileProxy::operator=(const RemoteFileProxy& file) {
  FileProxy::operator=(file);
  localUser = file.localUser;
  upToDate = file.isUpToDate();
  return *this;
}

/* Call the change group DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::chgrp(const string& group) {
  diet_profile_t* profile;
  char* errMsg;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  profile = diet_profile_alloc(CHGRP_SRV(getHost()), 3, 3, 4);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 3), const_cast<char*>(group.c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);

  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to change file group");
  diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);

  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  setGroup(group);
  return 0;
}

/* Call the change mode DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::chmod(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  long m = mode;
  sysEndianChg<long>(m);
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");

  profile = diet_profile_alloc(CHMOD_SRV(getHost()), 3, 3, 4);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 3), &m, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to change file mode");
  diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  setPerms(mode);
  
  return 0;
}

/* Call the file head DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
string RemoteFileProxy::head(const unsigned int nline) {
  string result;
  char* fileHead, * errMsg;
  diet_profile_t* profile;
  unsigned long n = nline;
  
  std::string serviceName("FileHead");
  std::string sessionKey=this->getSession().getSessionKey();
  
  sysEndianChg<unsigned long>(n);
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");

  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
 
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(sessionKey.c_str()),
                  DIET_VOLATILE);
 
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 4), &n, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");

  diet_string_get(diet_parameter(profile, 5), &fileHead, NULL);
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);

  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  
  result = fileHead;
  return result;
}

/* Call the mkdir DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::mkdir(const mode_t mode) {
  diet_profile_t* profile;
  char* errMsg;
  long m = mode;
  
  sysEndianChg<long>(m);
  
  if (exists())
    throw runtime_error(getPath()+ " already exists");
  
  profile = diet_profile_alloc(MKDIR_SRV(getHost()), 3, 3, 4);
  
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 3), &m, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  diet_string_get(diet_parameter(profile, 4), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }  
  
  exists(true);
  return 0;  
}


/* Call the remove file DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::rm() {
  diet_profile_t* profile;
  char* errMsg;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  profile = diet_profile_alloc(RM_SRV(getHost()), 2, 2, 3);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 3), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  diet_string_get(diet_parameter(profile, 3), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }  
  
  exists(true);
  return 0;
}

/* Call the remove directory DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFileProxy::rmdir() {
  diet_profile_t* profile;
  char* errMsg;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  profile = diet_profile_alloc(RMDIR_SRV(getHost()), 2, 2, 3);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 3), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  diet_string_get(diet_parameter(profile, 3), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }  
  
  exists(true);
  return 0;
}

/* Call the file tail DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
string RemoteFileProxy::tail(const unsigned int nline) {
  string result;
  char* fileTail, * errMsg;
  diet_profile_t* profile;
  unsigned long n = nline;
  
  std::string serviceName("FileTail");
  std::string sessionKey=this->getSession().getSessionKey();
  
  sysEndianChg<unsigned long>(n);
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  profile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);
  
  diet_string_set(diet_parameter(profile, 0), strdup(sessionKey.c_str()),
                  DIET_VOLATILE);
  
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 2), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 3), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 4), &n, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 5), &fileTail, NULL);
  diet_string_get(diet_parameter(profile, 6), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  
  result = fileTail;
  return result;
}


void RemoteFileProxy::printTransferID(const bool printTrID) {
  this->printTrID=printTrID;
}
