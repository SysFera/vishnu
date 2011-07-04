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
makeExportOpt(string pgName,
            boost::function1<void, IMS_Data::ExportType>& fExportType,
            string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

    // All cli options
   opt->add("exportType,t",
            "the type to export\n"
            "1 for SHELL",
            CONFIG,
            fExportType);

  return opt;
}


int main (int argc, char* argv[]){

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string oldSessionId;
  string filename;

   /********** EMF data ************/
  IMS_Data::ExportOp exportOp;

  /******** Callback functions ******************/
  boost::function1<void, IMS_Data::ExportType> fExportType (boost::bind(&IMS_Data::ExportOp::setExportType,boost::ref(exportOp),_1));

  /*********** Out parameters *********************/

  /**************** Describe options *************/


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeExportOpt(argv[0], fExportType, dietConfig);

  opt->add( "oldSessionId,o",
            "represents the id of the session to export",
            HIDDEN,
            oldSessionId,1);

  opt->setPosition("oldSessionId",1);

  opt->add( "filename,f",
            "represents the file in which the data will be exported",
            HIDDEN,
            filename,1);

  opt->setPosition("filename",1);

  CLICmd cmd = CLICmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] oldSessionId filename");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] oldSessionId filename");
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
      exportCommands(sessionKey, oldSessionId, filename, exportOp);
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

