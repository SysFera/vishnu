/**
 * \file get_system_info.cpp
 * This file defines the VISHNU command to get the system information
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
 */



#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "displayer.hpp"


using namespace std;
using namespace vishnu;

boost::shared_ptr<Options>
makeGetystemInfoOpt(string pgName,
                  boost::function1<void, string>& fMachineId,
                  string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

   opt->add("machineId,m",
            "represents the id of the machine",
            CONFIG,
            fMachineId);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;

   /********** EMF data ************/
  IMS_Data::SysInfoOp sysInfoOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fMachineId (boost::bind(&IMS_Data::SysInfoOp::setMachineId,boost::ref(sysInfoOp),_1));

  /*********** Out parameters *********************/
  IMS_Data::ListSysInfo listSysInfo;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeGetystemInfoOpt(argv[0], fMachineId, dietConfig);

  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] ");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options]");
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

    // DIET call : get job output
    if(false==sessionKey.empty()){
      printSessionKeyMessage();
      getSystemInfo(sessionKey, listSysInfo, sysInfoOp);
      displayListSysInfo(&listSysInfo);
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

