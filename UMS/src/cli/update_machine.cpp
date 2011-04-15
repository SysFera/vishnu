/**
 * \file update_machine.cpp
 * This file defines the VISHNU update machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "machineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sshPublicKeyPath;

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::Machine upMachine;


  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(upMachine),_1));

  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(upMachine),_1));
  
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::Machine::setMachineId,boost::ref(upMachine),_1));

  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(upMachine),_1));

  boost::function1<void,string> fMachineDescription( boost::bind(&UMS_Data::Machine::setMachineDescription,boost::ref(upMachine),_1));

  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::Machine::setStatus,boost::ref(upMachine),_1));

  // Describe options
  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage,sshPublicKeyPath,fMachineDescription);


  opt->add("machineId",
           "The identifier of the machine",
           HIDDEN,
           fMachineId,
           1);

  opt->setPosition("machineId",-1);

  opt->add("status,t",
           "The status of the machine",
           CONFIG,
           fStatus);


  try{


    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();

    /********  Process **************************/


    checkVishnuConfig(*opt);
    
    if(opt->count("sshPublicKeyFile")){

      // read the public key file from the public key path and set the neMachine

      upMachine.setSshPublicKey(get_file_content(sshPublicKeyPath));


    }


    /************** Call UMS update machine service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  CLI_ERROR_DIET ;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      updateMachine(sessionKey,upMachine);

      printSuccessMessage();

    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," machineId ",requiredParamMsg);

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

  catch(std::exception& e){

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


