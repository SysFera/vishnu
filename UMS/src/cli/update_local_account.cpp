/**
 * \file update_local_account.cpp
 * This file defines the VISHNU update local account command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "localAccountUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sessionKey;


  /********** EMF data ************/

  UMS_Data::LocalAccount upAcLogin;
  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(upAcLogin),_1));
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(upAcLogin),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,dietConfig,fMachineId,
                                                         fAcLogin,fSshKeyPath,fHomeDirectory);

  try {
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/


    checkVishnuConfig(*opt);


    /************** Call UMS update local account service************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
    
      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return CLI_ERROR_DIET ;
    }


    //get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      updateLocalAccount(sessionKey,upAcLogin);

      printSuccessMessage();

    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," userId machineId",requiredParamMsg);

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

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


