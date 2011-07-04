/**
 * \file tail_of_file.cpp
 * This file defines the VISHNU  tail of file command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "FMS_Data.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

/**
 * \brief To build options for the VISHNU tail of file command
 * \param pgName : The name of the command
 * \param path : the path of the file to display
 * \param dietConfig: Represents the VISHNU config file
 */
boost::shared_ptr<Options>
makeFMSCommonOpt(string pgName, string& path,
              string& dietConfig){
 
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);
  
  // All cli options
  
  opt->add("path,p",
      "represents the path of the file to display",
      HIDDEN,
      path,1);

  opt->setPosition("path",1);

  return opt;
}


int main (int ac, char* av[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string path;

  /********** EMF data ************/
  
  TailOfFileOptions tofOptions;
  
  /******** Callback functions ******************/

  boost::function1<void,int> fNline(boost::bind(&FMS_Data::TailOfFileOptions::setNline,boost::ref(tofOptions),_1));
     
  /*********** Out parameters *********************/

  string contentOfFile;
  
  /**************** Describe options *************/
  
  boost::shared_ptr<Options> opt ( makeFMSCommonOpt( av[0], path, dietConfig )   );
  
  opt->add("nline,n",
      "The line to display",
      CONFIG,
      fNline);

  CLICmd cmd = CLICmd (ac, av, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] path");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] path");
    return 0;
  }


if (opt->count ("nline")) cout << "nline: " << tofOptions.getNline() << "\n";
if (opt->count ("path")) cout << "path: " << path << "\n";



  // Process command
  try {

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      errorUsage(av[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : tail_of_file
    if(false==sessionKey.empty()){
      printSessionKeyMessage();
            tailOfFile(sessionKey,path,contentOfFile, tofOptions);
            cout << contentOfFile << "\n";
    }
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return CLI_ERROR_RUNTIME;
  }
  return 0;
}
