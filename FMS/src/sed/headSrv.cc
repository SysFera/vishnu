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


/// DIET profile construction.
diet_profile_desc_t* getHeadProfile() {
  diet_profile_desc_t* result = diet_profile_desc_alloc("FileHead", 4, 4, 6);
  
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* head DIET callback function. Proceed to the group change using the
 client parameters. Returns an error message if something gone wrong. */
/* Returns the n first line of the file to the client application. */
int headFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, head, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *errMsg = NULL, *result = NULL, *optionsSerialized=NULL;
  
  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4), &optionsSerialized, NULL);


  std::cout << "Dans headFile:  " << "\n"; 
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

    HeadOfFileOptions_ptr options_ptr= NULL;
 if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Head: HeadOfFileOptions object is not well built");
    }


      head = file->head(*options_ptr);
      result = strdup(head.c_str());

    } catch (std::exception& err) {
      result = strdup("");
      errMsg = strdup(err.what());
    }
    if (errMsg==NULL) {
      errMsg = strdup("");
    }
    else {
     result = strdup("");
    }
    diet_string_set(diet_parameter(profile, 5), result, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile, 6), errMsg, DIET_VOLATILE);
    return 0;
}
