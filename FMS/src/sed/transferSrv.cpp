#include <string>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include <sys/types.h>

#include "FileFactory.hpp"

#include "DIET_client.h"
#include "UserServer.hpp"
#include "MachineServer.hpp"
#include <boost/scoped_ptr.hpp>
#include "transferSrv.hpp"
using namespace std;


/**
 * \brief Function to solve the getListOfJobs service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfFileTransfers(diet_profile_t* pb) {
  return solveGenerique<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList, ListFileTransfers >(pb);
}



/**
 * \brief Function to solve the File transfer stop service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveFileTransferStop(diet_profile_t* profile) {

  std::string sessionKey = "";
  std::string optionsSerialized = "";
  std::string cmd = "";

  diet_string_get(profile,0, sessionKey);
  diet_string_get(profile,1, optionsSerialized);

  // reset the profile to handle result
  diet_profile_reset(profile, 2);

  SessionServer sessionServer = SessionServer(sessionKey);
  FMS_Data::StopTransferOptions_ptr options_ptr = NULL;

  try {
    //MAPPER CREATION
    int mapperkey;
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(vishnu::FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_stop_file_transfer");
    mapper->code(optionsSerialized, mapperkey);
    cmd = mapper->finalize(mapperkey);

    if(!vishnu::parseEmfObject(optionsSerialized, options_ptr)) {
      SystemException(ERRCODE_INVDATA, "solve_fileTransferStop: options object is not well built");
    }

    int vishnuId = ServerFMS::getInstance()->getVishnuId();
    boost::shared_ptr<FileTransferServer> fileTransferServer(new FileTransferServer(sessionServer,vishnuId));
    fileTransferServer->stopThread(*options_ptr);

    // set success result
    diet_string_set(profile, 0, "success");
    diet_string_set(profile, 1, "");
    sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& err) {
    try {
      sessionServer.finish(cmd, vishnu::FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      err.appendMsgComp(fe.what());
    }
    // set error result
    diet_string_set(profile, 0, "error");
    diet_string_set(profile, 1, err.what());
  }
  return 0;
}
