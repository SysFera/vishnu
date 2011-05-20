#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "FileFactory.hh"

#include "DIET_server.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"


using namespace std;

/* Global variables defined in the server.cc file. */

/* DIET profile construction.
 * Use the serverHostname global variable to create the service name. */
diet_profile_desc_t* getTailProfile() {
  diet_profile_desc_t* result = diet_profile_desc_alloc("FileTail", 4, 4, 6);
  
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* tail DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n last lines of a file to the client application. */
int tailFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, tail, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *errMsg = NULL, *result = NULL, *optionsSerialized= NULL;
  unsigned long* nline;
  
  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4),&optionsSerialized, NULL);
  
  sysEndianChg<unsigned long>(*nline);
  
  std::cout << "Dans tailFile:  " << "\n"; 
  std::cout << "path:  " << path <<"\n"; 
  std::cout << "user:  " << user <<"\n";
  std::cout << "host:  " << host <<"\n"; 


  localUser = user;
  localPath = path;


    try {
      
    std::cout << "Dans headFile, dans le try catch:  " << "\n";
    std::cout << "localPath:  " << localPath <<"\n";  
    std::cout << "localUser:  " << localUser <<"\n";
    std::cout << "userKey   " << userKey <<"\n";

    SessionServer sessionServer (sessionKey);

    // check the sessionKey
    
    sessionServer.check();
    
    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(host);
    // get the machineName
    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(host);
    MachineServer machineServer(machine);
    machineName = machineServer.getMachineName();
    delete machine;

    std::cout << "acLogin: " << acLogin << "\n";
    std::cout << "machineName: " << machineName << "\n";

    FileFactory::setSSHServer(machineName);
    
boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, acLogin, userKey));

    TailOfFileOptions_ptr options_ptr= NULL;
 if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr )) {
      throw SystemException(ERRCODE_INVDATA, "solve_Tail: TailOfFileOptions object is not well built");
    }

      tail = file->tail(*options_ptr);
      result = strdup(tail.c_str());
    
    } catch (exception& err) {
      result = strdup("");
      errMsg = strdup(err.what());
    }
    if (errMsg==NULL) errMsg = strdup("");
    else {
    errMsg = strdup("Error transmitting parameters");
    result = strdup("");
  }
  diet_string_set(diet_parameter(profile, 5), result, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 6), errMsg, DIET_VOLATILE);
  return 0;
}
