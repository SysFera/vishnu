/**
 * \file add_machine.cpp
 * This file defines the VISHNU add machine command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "utils.hpp"
#include "machineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){




  /******* Parsed value containers ****************/

  string dietConfig;

  std::string machineDescription;

  std::string sessionKey;

  std::string sshPublicKeyFile;

  std::string sshPublicKeyPath;


  /********** EMF data ************/

  UMS_Data::Machine newMachine;


  /**************** Callback functions *************/

  boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(newMachine),_1));
  boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(newMachine),_1));
  boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(newMachine),_1));

  boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage,1);


  opt->add("sshPublicKeyFile,k",
           "The the path to the SSH public key used by VISHNU to access local user accounts",
           HIDDEN,
           sshPublicKeyPath,
           1);

  opt->setPosition("sshPublicKeyFile",1);


  try{
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/


    checkVishnuConfig(*opt);

    // read the public key file from the public key path

    {
      std::ifstream ifs (sshPublicKeyPath.c_str(),std::ifstream::in);
      if (ifs.is_open()){
        ifs >>sshPublicKeyFile;
        newMachine.setSshPublicKey(sshPublicKeyFile);
        ifs.close();
      }
      else{
        std::cerr << "can not open the ssh public key file\n";
        return 1;
      }
    }


    cout << "Enter the Machine Description:\n ";

    getline(cin, machineDescription);

    newMachine.setMachineDescription(machineDescription);

    /************** Call UMS add machine service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      addMachine(sessionKey,newMachine);

      cout << "The machine identifier is " << newMachine.getMachineId() << endl;

    }



  }// End of try bloc


  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," name site language sshPublicKeyFile ","required parameter is missing");

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


