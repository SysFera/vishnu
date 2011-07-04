/**
 * \file ch_mod.cpp
 * This file defines the VISHNU ch_mod command 
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
#include "cmdArgs.hpp"
#include <boost/bind.hpp>
#include <sys/types.h>
#include "FMSDisplayer.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int argc, char* argv[]){
  
  int ret; // Return value
 // ChModOptions chOptions;

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string path;
  mode_t  mode;
  /******** Callback functions ******************/
  //boost::function1<void,string> fmode(boost::bind(&FMS_Data::ChModOptions::setMode,boost::ref(chOptions),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=processOpt(argv[0], dietConfig);

  opt->add("mode,m",
      "The acces rights of file/directory in octal sytem",
      HIDDEN,
      mode,1);
  opt->setPosition("mode",1); 

 opt->add("path,p",
      "The file/directory following the pattern [host:]file path",
      HIDDEN,
      path,1);
  opt->setPosition("path",1); 

  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt," mode path");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] mode path");
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
      chMod(sessionKey, mode, path);
    }
    printSuccessMessage();
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
