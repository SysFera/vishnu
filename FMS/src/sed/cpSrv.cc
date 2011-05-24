#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/types.h>

#include "FileFactory.hh"

#include "DIET_server.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>

using namespace std;


/// DIET profile construction.
diet_profile_desc_t* getCopyFileProfile() {
  diet_profile_desc_t* result = diet_profile_desc_alloc("FileCopy", 5, 5, 6);
  
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
  
  return result;
}

/* copy file DIET callback function. Returns an error message if something gone wrong. */

int solveCopyFile(diet_profile_t* profile) {
  string localPath, localUser, userKey, head, acLogin, machineName;
  char* path, *user, *host,*sessionKey, *dest, *errMsg = NULL, *result = NULL, *optionsSerialized=NULL;
  
  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);
  diet_string_get(diet_parameter(profile, 4), &dest, NULL);
  diet_string_get(diet_parameter(profile, 5), &optionsSerialized, NULL);


  std::cout << "Dans solveCopyFile:  " << "\n"; 
  std::cout << "path:  " << path <<"\n"; 
  std::cout << "user:  " << user <<"\n";
  std::cout << "host:  " << host <<"\n"; 
  std::cout << "dest:  " << dest <<"\n"; 


  localUser = user;
  localPath = path;

  std:: string destPath=File::extName(dest);
  std:: string destHost=File::extHost(dest);


  try {


    SessionServer sessionServer (sessionKey);

    // check the sessionKey
    
    sessionServer.check();
   


    UMS_Data::Machine_ptr machine = new UMS_Data::Machine();
    machine->setMachineId(destHost);
    MachineServer machineServer(machine);
    
    // check the machine
    machineServer.checkMachine();

    // get the machineName
    machineName = machineServer.getMachineName();
    delete machine;

    // get the acLogin
    acLogin = UserServer(sessionServer).getUserAccountLogin(destHost);

    std::cout << "acLogin: " << acLogin << "\n";
    std::cout << "machineName: " << machineName << "\n";

    FileFactory::setSSHServer(host);
    boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, localUser, userKey));

    CpFileOptions_ptr options_ptr= NULL;
    if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr) ) {
      throw SystemException(ERRCODE_INVDATA, "solve_Copy: CpFileOptions object is not well built");
    }

    std::ostringstream destCompletePath;
    destCompletePath << acLogin << "@"<<machineName <<":"<<destPath;
    std::cout << "destCompletePath " <<destCompletePath.str() << "\n";
    file->cp(destCompletePath.str(),*options_ptr);

  } catch (VishnuException& err) {
    errMsg = strdup(err.buildExceptionString().c_str());
  }
  if (errMsg==NULL) {
    errMsg = strdup("");
  }

    diet_string_set(diet_parameter(profile, 6), errMsg, DIET_VOLATILE);
    return 0;
}
