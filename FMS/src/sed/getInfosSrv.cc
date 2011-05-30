#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "DIET_server.h"
#include "FileFactory.hh"
#include "File.hh"
#include "services.hh"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>
#include "FMSVishnuException.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace FMS_Data;
namespace ba=boost::algorithm;
/// DIET profile construction.

diet_profile_desc_t* getInfosProfile() {
  diet_profile_desc_t* result;
  
  result = diet_profile_desc_alloc("FileGetInfos", 3, 3, 5);
  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
  
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
 
  char* path, * user, * host, *sessionKey, *fileStatSerialized=NULL;
  string localPath, localUser, userKey, machineName;
  char* errMsg = NULL;


  diet_string_get(diet_parameter(profile, 0), &sessionKey, NULL);
  diet_string_get(diet_parameter(profile, 1), &path, NULL);
  diet_string_get(diet_parameter(profile, 2), &user, NULL);
  diet_paramstring_get(diet_parameter(profile, 3), &host, NULL);

  std::cout << "Service get info, sessionKey : " << sessionKey << " bien recu\n";
  std:: cout << "path: " << path << "\n";
  std::cout << "user: "  << user << "\n";
  std::cout << "host: "  << host<< "\n";

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


        std::string acLogin = UserServer(sessionServer).getUserAccountLogin(host);

        std::cout << "acLogin: " << acLogin << "\n";
    
        FileFactory::setSSHServer(machineName);

        boost::scoped_ptr<File> file (FileFactory::getFileServer(sessionServer,localPath, acLogin, userKey));

        boost::scoped_ptr<FileStat> fileStat_ptr (new FileStat);

        if ( file->exists()) {

          *fileStat_ptr=file->getFileStat();
          const char* name = "solve_getInfos";
          ::ecorecpp::serializer::serializer _ser(name);
          fileStatSerialized = strdup(_ser.serialize(const_cast<FMS_Data::FileStat_ptr>(fileStat_ptr.get())).c_str());

        } else {
        
          throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, static_cast<SSHFile*>(file.get())->getErrorMsg());
        }
      } catch (VishnuException& err) {
        errMsg = strdup(err.buildExceptionString().c_str());
        fileStatSerialized=strdup(""); 
      }

  if (errMsg==NULL) errMsg = strdup("");
  

  diet_string_set(diet_parameter(profile, 4),fileStatSerialized, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 5), errMsg, DIET_VOLATILE);
  return 0;
}
