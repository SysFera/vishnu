#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "UserTable.hh"
#include "GrpTable.hh"
#include "FileFactory.hh"

#include "DIET_server.h"

using namespace std;

/* Global variables defined in the server.cc file. */
extern UserTable* users;
extern GroupTable* groups;
extern string serverHostname;

/* DIET profile construction.
 * Use the serverHostname global variable to create the service name. */
diet_profile_desc_t* getHeadProfile() {
  diet_profile_desc_t* result = diet_profile_desc_alloc(HEAD_SRV(serverHostname), 3, 3, 5);
  
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_SCALAR, DIET_LONGINT);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* head DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int headFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, head;
  char* path, *user, *host, *errMsg = NULL, *result = NULL;
  unsigned long* nline;
  
  diet_string_get(diet_parameter(profile, 0), &path, NULL);
  diet_string_get(diet_parameter(profile, 1), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 2), &host, NULL);
  diet_scalar_get(diet_parameter(profile, 3), &nline, NULL);
  
  sysEndianChg<unsigned long>(*nline);
  
  std::cout << "Dans headFile:  " << "\n"; 
  std::cout << "path:  " << path <<"\n"; 
  std::cout << "user:  " << user <<"\n";
  std::cout << "host:  " << host <<"\n"; 
 
   
   
    if (user!=NULL && path!=NULL && nline!=NULL) {
    try {
      if (path[0]!='/' && users)
        localPath = users->getHome(user) + "/" + path;
      else
        localPath = path;
      localUser = users->getLocalID(user);
      userKey = users->getKey(user);
    } catch (runtime_error& err) {
 
      std::cout << "Dans headFile, catch:  " << "\n";
      localUser = user;
      localPath = path;
    }
    try {
 std::cout << "Dans headFile, dans le try catch:  " << "\n";
  std::cout << "localPath:  " << localPath <<"\n";  
  std::cout << "localUser:  " << localUser <<"\n";
  std::cout << "userKey   " << userKey <<"\n";



      File* file = FileFactory::getFileServer(localPath, localUser, userKey);
      
      head = file->head(*nline);
      result = strdup(head.c_str());
    } catch (runtime_error& err) {
      result = strdup("");
      errMsg = strdup(err.what());
    }
    if (errMsg==NULL) errMsg = strdup("");
  } else {
    errMsg = strdup("Error transmitting parameters");
    result = strdup("");
  }
  diet_string_set(diet_parameter(profile, 4), result, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), errMsg, DIET_VOLATILE);
  return 0;
}
