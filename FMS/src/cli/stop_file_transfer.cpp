/**
 * \file stope_file_transfer.cpp
 * This file defines the VISHNU stop file transfer command 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "FMS_Data.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU list directory of file command
 * \param pgName : The name of the command
 * \param dietConfig: Represents the VISHNU config file
 * \param ftransferId: The file transfer identifier
 * \param ffromMachineId: The machine that is the source of the file transfer
 * \param fuserId: The user identifier
 */
boost::shared_ptr<Options>
makeStopFileTrOpt(string pgName, 
    string& dietConfig,
    boost::function1<void, string>& ftransferId,
    boost::function1<void, string>& ffromMachineId,
    boost::function1<void, string>& fuserId){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

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
  string dietConfig;
  string sessionKey;

   /********** EMF data ************/
  FMS_Data::StopTransferOptions stopFileTransferOptions;

  /******** Callback functions ******************/
  boost::function1<void, string> ftranferId(boost::bind(&FMS_Data::StopTransferOptions::setTransferId, boost::ref(stopFileTransferOptions),_1));
  boost::function1<void, string> ffromMachineId(boost::bind(&FMS_Data::StopTransferOptions::setFromMachineId, boost::ref(stopFileTransferOptions),_1));
  boost::function1<void, string> fuserId(boost::bind(&FMS_Data::StopTransferOptions::setUserId, boost::ref(stopFileTransferOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeStopFileTrOpt(argv[0], dietConfig, ftranferId, ffromMachineId, fuserId);

  CLICmd cmd = CLICmd (argc, argv, opt);

 // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());
  
  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] src dest");
    return 0;
  }

  // Process command
  try {

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call 
    if(false==sessionKey.empty()){
      printSessionKeyMessage();
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
