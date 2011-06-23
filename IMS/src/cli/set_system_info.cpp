/**
 * \file set_system_info.cpp
 * This file defines the VISHNU command to set the system information
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
makeSystemInfoOpt(string pgName,
                  boost::function1<void, long>& fMemory,
                  boost::function1<void, long>& fDiskSpace,
                  string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

   opt->add("memory,m",
            "Amount of disk space available on the machine (in Bytes)",
            CONFIG,
            fMemory);

  // All cli options
  opt->add("diskSpace,d",
           "Amount of RAM memory available on the machine (in Bytes)",
            CONFIG,
            fDiskSpace);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;

   /********** EMF data ************/
  IMS_Data::SystemInfo sysInfo;

  /******** Callback functions ******************/
  boost::function1<void,long> fMemory (boost::bind(&IMS_Data::SystemInfo::setMemory,boost::ref(sysInfo),_1));
  boost::function1<void,long> fDiskSpace (boost::bind(&IMS_Data::SystemInfo::setDiskSpace,boost::ref(sysInfo),_1));
  /*********** Out parameters *********************/

  /**************** Describe options *************/


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeSystemInfoOpt(argv[0], fMemory, fDiskSpace, dietConfig);

  opt->add( "machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);

  opt->setPosition("machineId",1);



  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());
  sysInfo.setMachineId(machineId);

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] machineId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] machineId");
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
      setSystemInfo(sessionKey, sysInfo);
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

