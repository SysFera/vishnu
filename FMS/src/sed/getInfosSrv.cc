#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "DIET_server.h"
#include "RemoteFile.hh"
#include "FileFactory.hh"
#include "File.hh"
#include "services.hh"

using namespace std;

/* Global variables defined in the server.cc file. */
//extern UserTable* users;
//extern string serverHostname;

/* DIET profile construction.
 * Use the serverHostname global variable to create the service name. */
diet_profile_desc_t* getInfosProfile() {
  diet_profile_desc_t* result;
  
  result = diet_profile_desc_alloc("FileGetInfos", 2, 2, 13);
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 6), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 7), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 8), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 9), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 10), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 11), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 12), DIET_SCALAR, DIET_INT);
  diet_generic_desc_set(diet_param_desc(result, 13), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* get information DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The function returns all the information about a file:
 *  - The local owner
 *  - The local group
 *  - The local uid & gid
 *  - The creation, modification and acces time.
 *  - The file type.
 */
int get_infos(diet_profile_t* profile) {
  char* path, * user, * host;
  string localPath, localUser, userKey;
  char* owner, * group, * errMsg = NULL;
  uid_t* uid;
  gid_t* gid;
  mode_t* perms;
  file_size_t* size;
  time_t* atime, * mtime, * ctime;
  int* type;
  File* file;
  
  diet_string_get(diet_parameter(profile, 0), &path, NULL);
  diet_string_get(diet_parameter(profile, 1), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 2), &host, NULL);


  if (user!=NULL && path!=NULL) {
    try {
      if (path[0]!='/'){
     //   localPath = users->getHome(user) + "/" + path;
      }
      else{
        localPath = path;
 //     localUser = users->getLocalID(user);
   //   userKey = users->getKey(user);
      }
    } catch (runtime_error& err) {
      localUser = user;
      localPath = path;
    }
  }

  try {
    file = FileFactory::getFileServer(localPath, localUser, userKey);
  } catch (runtime_error& err) {
    errMsg = strdup(err.what());
  }
  if (file->exists()) {
    owner = strdup(file->getOwner().c_str());
  
    group = strdup(file->getGroup().c_str());
    uid = uiddup(file->getUid());
    gid = giddup(file->getGid());
    perms = modedup(file->getPerms());
    size = sizedup(file->getSize());
    atime = timedup(file->getAtime());
    mtime = timedup(file->getMtime());
    ctime = timedup(file->getCtime());
    type = (int*) typedup(file->getType());
  } else {
    owner = strdup("");
    
    group = strdup("");
    uid = uiddup(0);
    gid = giddup(0);
    perms = modedup(0);
    size = sizedup(0);
    atime = timedup(0);
    mtime = timedup(0);
    ctime = timedup(0);
    type = (int*) typedup(regular);
  }

  if (errMsg==NULL) errMsg = strdup("");
  
  sysEndianChg<uid_t>(*uid);
  sysEndianChg<gid_t>(*gid);
  sysEndianChg<mode_t>(*perms);
  sysEndianChg<file_size_t>(*size);
  sysEndianChg<time_t>(*atime);
  sysEndianChg<time_t>(*mtime);
  sysEndianChg<time_t>(*ctime);
  sysEndianChg<int>(*type);

  diet_string_set(diet_parameter(profile, 3), owner, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 4), group, DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 5), uid, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 6), gid, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 7), perms, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 8), size, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 9), atime, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 10), mtime, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 11), ctime, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 12), type, DIET_VOLATILE, DIET_INT);
  diet_string_set(diet_parameter(profile, 13), errMsg, DIET_VOLATILE);
  return 0;
}
