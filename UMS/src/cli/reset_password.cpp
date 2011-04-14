/**
 * \file reset_password.cpp
 * This file defines the VISHNU reset password command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  string userId;

  string sessionKey;

  string dietConfig;


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

  opt->setPosition("userId",-1);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();

    /********  Process **************************/

    checkVishnuConfig(*opt);

    /************** Call UMS reset Password service *******************************/


    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  CLI_ERROR_DIET ;


    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      std::string tmpPassword;

      resetPassword(sessionKey,userId, tmpPassword);

      printSuccessMessage();
    }



  }// End of try bloc


  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] userId ",requiredParamMsg);

    return CLI_ERROR_MISSING_PARAMETER;
  }

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error


    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0], e.what());

    return CLI_ERROR_RUNTIME;

  }

  return 0;

}// end of main


