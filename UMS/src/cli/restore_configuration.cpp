/**
 * \file restore_configuration.cpp
 * This file defines the VISHNU restore configuration command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  string filePath;

  string sessionKey;

  string dietConfig;

  /**************** Describe options *************/

  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);


  opt.add("filePath",
          "The path of the VISHNU configuration file",
          HIDDEN,
          filePath,
          1);

  opt.setPosition("filePath",-1);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    opt.parse_env(env_name_mapper());

    opt.notify();


    /********  Process **************************/

    checkVishnuConfig(opt);

    /************** Call UMS restore configuration service *******************************/

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  CLI_ERROR_DIET ;

    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      restoreConfiguration(sessionKey, filePath);

      printSuccessMessage();
    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(opt,"[options] filePath ",requiredParamMsg);

    return CLI_ERROR_MISSING_PARAMETER;

  }

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    //check the bad session key
    
    if (checkBadSessionKeyError(e)){

      removeBadSessionKeyFromFile(getppid());
    }


    return e.getMsgI() ;
  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0], e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


