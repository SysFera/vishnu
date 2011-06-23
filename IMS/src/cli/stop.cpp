/**
 * \file stop.cpp
 * This file defines the VISHNU command to stop a vishnu process
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


using namespace std;
using namespace vishnu;

boost::shared_ptr<Options>
makeStopOpt(string pgName,
            boost::function1<void, string>& fDietId,
            string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

    // All cli options
   opt->add("dietId,d",
            "The diet id of the process",
            CONFIG,
            fDietId);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string processName;

   /********** EMF data ************/
  IMS_Data::Process process;

  /******** Callback functions ******************/
  boost::function1<void, string> fDietId (boost::bind(&IMS_Data::Process::setDietId,boost::ref(process),_1));

  /*********** Out parameters *********************/

  /**************** Describe options *************/


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeStopOpt(argv[0], fDietId, dietConfig);


  opt->add( "processName,p",
            "represents the name of the process",
            HIDDEN,
            processName,1);

  opt->setPosition("processName",1);

  opt->add( "machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);

  opt->setPosition("machineId",1);


  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  process.setMachineId(machineId);
  process.setProcessName(processName);

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] processName machineId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] processName machineId");
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
      stop(sessionKey, process);
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

