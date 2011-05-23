#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>

#include <sys/types.h>

#include "FileFactory.hh"

#include "DIET_server.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>


using namespace std;


/* DIET profile construction.
 * Use the serverHostname global variable to create the service name. */
diet_profile_desc_t* getRemoveDirProfile() {
  diet_profile_desc_t* result = diet_profile_desc_alloc("DirRemove", 3, 3, 4);
  
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* mkdir DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* The directory to create is passed as client parameter. */
int solveRemoveDir(diet_profile_t* profile) {
  string localPath, localUser, userKey, acLogin, machineName;
  char* path, *user, *host, *sessionKey, *errMsg = NULL;
  
  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);
  
  std::cout << "Dans RemoveDir:  " << "\n"; 
  std::cout << "path:  " << path <<"\n"; 
  std::cout << "user:  " << user <<"\n";
  std::cout << "host:  " << host <<"\n"; 


      localUser = user;
      localPath = path;
    try {
    
      SessionServer sessionServer (sessionKey);

  // check the sessionKey
    
    sessionServer.check();
   // 
    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(host);
    MachineServer machineServer(machine);
    
    // check the machine
    machineServer.checkMachine();

    // get the machineName
    machineName = machineServer.getMachineName();
    delete machine;

    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(host);
       
    std::cout << "acLogin: " << acLogin << "\n";
    std::cout << "machineName: " << machineName << "\n";

    FileFactory::setSSHServer(machineName);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, acLogin, userKey));

    file->rmdir();
    } catch (std::exception& err) {
      errMsg = strdup(err.what());
    }
    if (errMsg==NULL){
      errMsg = strdup("");
    }

diet_string_set(diet_parameter(profile, 4), errMsg, DIET_VOLATILE);
  return 0;
}
