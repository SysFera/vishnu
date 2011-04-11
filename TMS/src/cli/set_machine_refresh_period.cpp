/**
 * \file set_machine_refresh_period.cpp
 * This file defines the VISHNU command to set the refreshing period of a machine
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "utils.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;



int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  int    value;

  /********** EMF data ************/

  /******** Callback functions ******************/
     
  /*********** Out parameters *********************/

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli obligatory parameters
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  opt->add("value,v",
	   "represents the value of the frequency",
	   HIDDEN,
	   value,1);
  opt->setPosition("value",1);


  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper(), "vishnu_set_machine_refresh_period machineId value");

  if (ret != CLI_SUCCESS){
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options]  ");  
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
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      setMachineRefreshPeriod(sessionKey, machineId, value);
    }

  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(argv[0], msg,EXECERROR);
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
