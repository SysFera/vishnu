/**
 * \file create_file.cpp
 * This file defines the VISHNU submit job command 
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

boost::shared_ptr<Options>
makeCopyOpt(string pgName, 
    string& dietConfig,
    boost::function1<void, int>& ftrCmd,
    string& src,
    string& dest,
    CpFileOptions& cpFileOptions){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("isRecursive,r",
      "It specifies when the copy is recursive (case of directory) or not.",
      CONFIG);
  
  if(opt->count("isRecursive")){
    cpFileOptions.setIsRecursive(true);
  }

  opt->add("trCommand,t",
      "The command to use to perform file transfer.",
      CONFIG,
      ftrCmd);

  opt->add("src, s",
      "The source file to copy following the pattern [host:]file path.",
      HIDDEN,
      src,1);
  opt->setPosition("src",1);

  opt->add("dest, d",
      "The path of the destination file.",
      HIDDEN,
      dest,1);
  opt->setPosition("dest",1);

  return opt;
}


int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string src;
  string dest;

   /********** EMF data ************/
  FMS_Data::CpFileOptions cpFileOptions;

  /******** Callback functions ******************/
  boost::function1<void,int> ftrCmd(boost::bind(&FMS_Data::CpFileOptions::setTrCommand,boost::ref(cpFileOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeCopyOpt(argv[0], dietConfig, ftrCmd, src, dest, cpFileOptions);

  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

 // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());
  
  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] src dest");
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
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      copyFile(sessionKey, src, dest, cpFileOptions);
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
