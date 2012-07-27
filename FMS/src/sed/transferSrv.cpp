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

//diet_profile_desc_t* getTransferFileProfile(const std::string& serviceName){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc(serviceName.c_str(), 5, 5, 6);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
//
//  return result;
//
//
//
//}

//diet_profile_desc_t* getTransferFileAsyncProfile(const std::string& serviceName){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc(serviceName.c_str(), 5, 5, 7);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 4), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 7), DIET_STRING, DIET_CHAR);
//
//
//  return result;
//
//}
//
//diet_profile_desc_t* getTransferRemoteFileProfile(const std::string& serviceName){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc(serviceName.c_str(), 6, 6, 7);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 4), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 7), DIET_STRING, DIET_CHAR);
//
//  return result;
//
//}
//
//diet_profile_desc_t* getFileTransfersListProfile(){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc("FileTransfersList", 1, 1, 3);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_STRING, DIET_CHAR);
//
//  return result;
//
//}
//
//diet_profile_desc_t* getTransferRemoteFileAsyncProfile(const std::string& serviceName){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc(serviceName.c_str(), 6, 6, 8);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 3), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 4), DIET_PARAMSTRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 5), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 6), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 7), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 8), DIET_STRING, DIET_CHAR);
//
//  return result;
//
//}
//
/**
 * \brief Function to solve the getListOfJobs service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveGetListOfFileTransfers(diet_profile_t* pb) {
  return solveGenerique<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList, ListFileTransfers >(pb);
}

//diet_profile_desc_t* getFileTransferStopProfile(){
//
//  diet_profile_desc_t* result = diet_profile_desc_alloc("FileTransferStop", 1, 1, 2);
//
//  diet_generic_desc_set(diet_param_desc(result, 0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(result, 2), DIET_STRING, DIET_CHAR);
//
//  return result;
//}


/**
 * \brief Function to solve the File transfer stop service
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int
solveFileTransferStop(diet_profile_t* pb) {

  char* sessionKey = NULL;
  char* optionsSerialized = NULL;
  std::string finishError ="";
  int mapperkey;
  std::string cmd = "";
  std::string errorInfo ="";

  diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
  diet_string_get(diet_parameter(pb,1), &optionsSerialized, NULL);

  SessionServer sessionServer = SessionServer(std::string(sessionKey));
  FMS_Data::StopTransferOptions_ptr options_ptr = NULL;

  try {
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(FMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_stop_file_transfer");
    mapper->code(std::string(optionsSerialized), mapperkey);
    cmd = mapper->finalize(mapperkey);

    if(!vishnu::parseEmfObject(std::string(optionsSerialized), options_ptr)) {
      SystemException(ERRCODE_INVDATA, "solve_fileTransferStop: options object is not well built");
    }

    int vishnuId=ServerFMS::getInstance()->getVishnuId();

    boost::shared_ptr<FileTransferServer> fileTransferServer(new FileTransferServer(sessionServer,vishnuId));

    fileTransferServer->stopThread(*options_ptr);

    diet_string_set(diet_parameter(pb,2), const_cast<char*>(errorInfo.c_str()), DIET_VOLATILE);
    sessionServer.finish(cmd, FMS, vishnu::CMDSUCCESS);
  } catch (VishnuException& e) {
    try {
      sessionServer.finish(cmd, FMS, vishnu::CMDFAILED);
    } catch (VishnuException& fe) {
      finishError =  fe.what();
      finishError +="\n";
    }
    e.appendMsgComp(finishError);
    errorInfo =  e.buildExceptionString();
    diet_string_set(diet_parameter(pb,2), const_cast<char*>(errorInfo.c_str()), DIET_VOLATILE);
  }
  return 0;
}

