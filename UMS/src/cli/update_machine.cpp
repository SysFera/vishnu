/**
 * \file update_machine.cpp
 * This file defines the VISHNU update machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
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

  std::string machineDescription;

  std::string sessionKey;


  /********** EMF data ************/

  UMS_Data::Machine upMachine;



  /**************** Describe options *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(upMachine),_1));

  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(upMachine),_1));
  
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::Machine::setMachineId,boost::ref(upMachine),_1));

  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(upMachine),_1));

  boost::function1<void,string> fSshPublicKeyFile( boost::bind(&UMS_Data::Machine::setSshPublicKey,boost::ref(upMachine),_1));

  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::Machine::setStatus,boost::ref(upMachine),_1));


  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage);


  opt->add("machineId",
           "The identifier of the machine",
           HIDDEN,
           fMachineId,
           1);

  opt->setPosition("machineId",-1);

  opt->add("machineDescription,d",
           "The status of the machine",
           CONFIG,
           fStatus);

  opt->add("status,t",
           "The description of the machine",
           CONFIG,
           fStatus);

  opt->add("sshPublicKeyFile,k",
           "The the path to the SSH public key used by VISHNU to access local user accounts",
           CONFIG,
           fSshPublicKeyFile);

  try{


    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();

    /********  Process **************************/


    if(opt->count("machineDescription")){//Fix me

      cout << "Enter the Machine Description:\n ";


      getline(cin, machineDescription);

      upMachine.setMachineDescription(machineDescription);

    }


    checkVishnuConfig(*opt);

    /************** Call UMS update service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      updateMachine(sessionKey,upMachine);


    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," machineId ","required parameter is missing");
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){

    errorUsage(av[0],e.what());

    return 1;
  }

  return 0;

}// end of main


