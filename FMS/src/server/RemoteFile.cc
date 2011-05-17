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

#include "RemoteFile.hh"
#include "LocalFile.hh"

using namespace std;

/* Default constructor. */
RemoteFile::RemoteFile() {
  upToDate = false;
}

/* Standard constructor.
 * Take the file path and user name as parameters.
 */
RemoteFile::RemoteFile(const SessionServer& sessionServer,const string& path,
                       const string& localUser) : File(sessionServer,path)
{
  upToDate = false;
  this->localUser = localUser;
}

/* Copy constructor. */
RemoteFile::RemoteFile(const RemoteFile& file) : File(file) {
  upToDate = false;
  this->localUser = file.localUser;
}

/* Standard destructor. */
RemoteFile::~RemoteFile() {
}

/* Returns true if the file informations are up to date. */
bool RemoteFile::isUpToDate() const {
  return upToDate;
}

/* Get the informations about this remote file. Call the DIET service. */
void RemoteFile::getInfos() const {
  diet_profile_t* profile;
  char* owner, * group, * errMsg;
  long* uid, * gid;
  long* perms, * size;
  long* atime, * mtime, * ctime;
  int* type;
  
  profile = diet_profile_alloc(GETINFOS_SRV(getHost()), 2, 2, 13);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 3), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 6), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 7), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 8), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 9), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 10), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 11), NULL, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 12), NULL, DIET_VOLATILE, DIET_INT);
  diet_string_set(diet_parameter(profile, 13), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service to obtain file informations");
  diet_string_get(diet_parameter(profile, 3), &owner, NULL);
  diet_string_get(diet_parameter(profile, 4), &group, NULL);
  diet_scalar_get(diet_parameter(profile, 5), &uid, NULL);
  diet_scalar_get(diet_parameter(profile, 6), &gid, NULL);
  diet_scalar_get(diet_parameter(profile, 7), &perms, NULL);
  diet_scalar_get(diet_parameter(profile, 8), &size, NULL);
  diet_scalar_get(diet_parameter(profile, 9), &atime, NULL);
  diet_scalar_get(diet_parameter(profile, 10), &mtime, NULL);
  diet_scalar_get(diet_parameter(profile, 11), &ctime, NULL);
  diet_scalar_get(diet_parameter(profile, 12), &type, NULL);
  diet_string_get(diet_parameter(profile, 13), &errMsg, NULL);

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
  setType((file_type_t) *type);
  exists(true);
  upToDate = true;
}

/* Copy operator. */
RemoteFile& RemoteFile::operator=(const RemoteFile& file) {
  File::operator=(file);
  localUser = file.localUser;
  upToDate = file.isUpToDate();
  return *this;
}

/* Call the change group DIET server.
 * If something goes wrong, throw a runtime_error containing
 * the error message.
 */
int RemoteFile::chgrp(const string& group) {
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
int RemoteFile::chmod(const mode_t mode) {
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
string RemoteFile::head(const unsigned int nline) {
  string result;
  char* fileHead, * errMsg;
  diet_profile_t* profile;
  unsigned long n = nline;
  
  sysEndianChg<unsigned long>(n);
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");

  profile = diet_profile_alloc(HEAD_SRV(getHost()), 3, 3, 5);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 3), &n, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");

  diet_string_get(diet_parameter(profile, 4), &fileHead, NULL);
  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);

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
int RemoteFile::mkdir(const mode_t mode) {
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
int RemoteFile::rm() {
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
int RemoteFile::rmdir() {
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
string RemoteFile::tail(const unsigned int nline) {
  string result;
  char* fileTail, * errMsg;
  diet_profile_t* profile;
  unsigned long n = nline;
  
  sysEndianChg<unsigned long>(n);
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  profile = diet_profile_alloc(TAIL_SRV(getHost()), 3, 3, 5);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(localUser.c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 3), &n, DIET_VOLATILE,
                  DIET_LONGINT);
  diet_string_set(diet_parameter(profile, 4), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  
  diet_string_get(diet_parameter(profile, 4), &fileTail, NULL);
  diet_string_get(diet_parameter(profile, 5), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  
  result = fileTail;
  return result;
}




void RemoteFile::printTransferID(const bool printTrID) {
  this->printTrID=printTrID;
}
