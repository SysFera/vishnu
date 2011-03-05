/**
 * \file add_local_account.cpp
 * This file defines the VISHNU add local account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "utils.hpp"
#include "localAccountUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string dietConfig;
  string sessionKey;

  /********** EMF data ************/

  UMS_Data::LocalAccount newAcLogin;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(newAcLogin),_1));

  /*****************Out parameters***/

  std::string sshPublicKey;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,dietConfig,fMachineId,
                                                         fAcLogin,fSshKeyPath,fHomeDirectory,1);

  try {
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();

    /********  Process **************************/


    checkVishnuConfig(*opt);

    /************** Call UMS add local account service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }


    //get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      addLocalAccount(sessionKey,newAcLogin,sshPublicKey);// call the UMS add local account service

       //To construct the file to save
      boost::filesystem::path home_dir = getenv("HOME");
      boost::filesystem::path  config_dir = home_dir;
      config_dir /= ".vishnu";
      config_dir /= "localAccountPublicKey";
    

      if(!boost::filesystem::exists(config_dir)){
        boost::filesystem::create_directories(config_dir);
      }
     
      std::string publicKeyName;
      publicKeyName.append(config_dir.string()+"/");
      publicKeyName.append(newAcLogin.getUserId());
      publicKeyName.append("-"+newAcLogin.getMachineId());
      
      ofstream os(publicKeyName.c_str());
      os << sshPublicKey;
      
      std::cout << "The ssh public key path is  " << publicKeyName << std::endl;
    
      printSuccessMessage();

    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing


    usage(*opt," userId machineId acLogin sshKeyPath homeDirectory ","required parameter is missing");
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


