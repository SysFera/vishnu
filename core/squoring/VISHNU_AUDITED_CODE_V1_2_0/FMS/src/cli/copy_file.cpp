/**
 * \file copy_file.cpp
 * This file defines the VISHNU copy file command 
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
 * \brief To build options for the VISHNU copy of file command
 * \param pgName: The name of the command
 * \param dietConfig: Represents the VISHNU config file
 * \param trCmdStr: The command to use to perform file transfer
 * \param src: The source file to copy following the pattern [host:]file path
 * \param dest: The path of the destination file
 */
boost::shared_ptr<Options>
makeCopyOpt(string pgName, 
    string& dietConfig,
    string& trCmdStr,
    string& src,
    string& dest){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("isRecursive,r",
      "It specifies when the copy is recursive (case of directory) or not.",
      CONFIG);
  
  opt->add("trCommand,t",
      "The command to use to perform file transfer. The different values  are:\n"
      "O or scp: for SCP transfer\n"
      "1 or rsync: for RSYNC transfer\n", 
      CONFIG,
      trCmdStr);

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
  string trCmdStr;
  int trCmd;

   /********** EMF data ************/
  FMS_Data::CpFileOptions cpFileOptions;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeCopyOpt(argv[0], dietConfig, trCmdStr, src, dest);

  CLICmd cmd = CLICmd (argc, argv, opt);

 // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if(trCmdStr.size()!=0) {
    size_t pos = trCmdStr.find_first_not_of("0123456789");
    if(pos!=std::string::npos) {
      if(trCmdStr.compare("scp")==0 || trCmdStr.compare("SCP")==0){
        trCmd = 0;
      } else if(trCmdStr.compare("rsync")==0 || trCmdStr.compare("RSYNC")==0){
        trCmd = 1;
      } else {
        std::cerr << "Unknown file transfer command type " << trCmdStr << std::endl;
        return 0;
      }
    } else {
      trCmd = convertToInt(trCmdStr);
    }
    cpFileOptions.setTrCommand(trCmd);
  }

  if(opt->count("isRecursive")){
    cpFileOptions.setIsRecursive(true);
  }

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
      printSessionKeyMessage();
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
