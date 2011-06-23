/**
 * \file set_system_threshold.cpp
 * This file defines the VISHNU command to set the system threshold
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"

using namespace std;
using namespace vishnu;

int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string handler;
  int value;
  IMS_Data::MetricType type;
   /********** EMF data ************/
  IMS_Data::Threshold systemThreshold;

  /**************** Describe options *************/


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  //Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);
  opt->setPosition("machineId",1);

  opt->add("value,v",
            "represents the value of the threshold",
            HIDDEN,
            value,1);
  opt->setPosition("value",1);

  opt->add("type,t",
            "represents the type of the threshold",
            HIDDEN,
            type,1);
  opt->setPosition("type",1);

  opt->add( "handler,u",
            "represents the userId of the administrator that handles the threshold",
            HIDDEN,
            handler,1);
  opt->setPosition("handler",1);


  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  //To set the corresponding values to the object systemThreshold
  systemThreshold.setMachineId(machineId);
  systemThreshold.setValue(value);
  systemThreshold.setType(type);
  systemThreshold.setHandler(handler);

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"machineId value type handler");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"machineId value type handler");
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
      setSystemThreshold(sessionKey, systemThreshold);
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

