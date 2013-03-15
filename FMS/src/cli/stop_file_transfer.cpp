/**
 * \file stope_file_transfer.cpp
 * This file defines the VISHNU stop file transfer command
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "FMS_Data.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU list directory of file command
 * \param pgName : The name of the command
 * \param configFile: Represents the VISHNU config file
 * \param ftransferId: The file transfer identifier
 * \param ffromMachineId: The machine that is the source of the file transfer
 * \param fuserId: The user identifier
 */
boost::shared_ptr<Options>
makeStopFileTrOpt(string pgName,
    string& configFile,
    boost::function1<void, string>& ftransferId,
    boost::function1<void, string>& ffromMachineId,
    boost::function1<void, string>& fuserId){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
      "VISHNU configuration file",
      ENV,
      configFile);

  opt->add("transferId,i",
      "A given transfer id",
      CONFIG,
      ftransferId);

 opt->add("fromMachineId,m",
      "The machine that is the source or destination of the file transfer",
      CONFIG,
      ffromMachineId);

 opt->add("userId,u",
      "Allows an admin to stop file transfers of a specific user",
      CONFIG,
      fuserId);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string configFile;
  string sessionKey;

   /********** EMF data ************/
  FMS_Data::StopTransferOptions stopFileTransferOptions;

  /******** Callback functions ******************/
  boost::function1<void, string> ftranferId(boost::bind(&FMS_Data::StopTransferOptions::setTransferId, boost::ref(stopFileTransferOptions),_1));
  boost::function1<void, string> ffromMachineId(boost::bind(&FMS_Data::StopTransferOptions::setFromMachineId, boost::ref(stopFileTransferOptions),_1));
  boost::function1<void, string> fuserId(boost::bind(&FMS_Data::StopTransferOptions::setUserId, boost::ref(stopFileTransferOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeStopFileTrOpt(argv[0], configFile, ftranferId, ffromMachineId, fuserId);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  // Process command
  try {
    // initializing VISHNU
    if (vishnuInitialize(const_cast<char*>(configFile.c_str()), argc, argv)) {
      errorUsage(argv[0],communicationErrorMsg,EXECERROR);
      return  CLI_ERROR_COMMUNICATION ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // VISHNU call
    if(false==sessionKey.empty()){
      stopFileTransfer(sessionKey, stopFileTransferOptions);
    }
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(argv[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
