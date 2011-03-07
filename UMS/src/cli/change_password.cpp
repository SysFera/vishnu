/**
 * \file change_password.cpp
 * This file defines the VISHNU change password command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  string userId;

  string oldPassword;

  string newPassword;

  string sessionKey;

  string dietConfig;

  /**************** Describe options *************/

  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt.add("userId",
          "The VISHNU user identifier",
          HIDDEN,
          userId,
          1);
  opt.setPosition("userId",-1);


  try {


    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    opt.parse_env(env_name_mapper());

    opt.notify();


    /********  Process **************************/

    if (opt.count("help")){

      helpUsage(opt,"[options]");

      return 0;

    }

    checkVishnuConfig(opt);

    oldPassword=vishnu::takePassword("old password: ");

    newPassword=vishnu::takePassword("new password: ");

    /************** Call UMS change password service *******************************/


    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  CLI_ERROR_DIET ;

    }

    changePassword(userId,oldPassword, newPassword);

    printSuccessMessage();

  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(opt,"[options] userId ",requiredParamMsg);

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


