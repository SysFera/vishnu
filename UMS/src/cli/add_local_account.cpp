/**
 * \file add_local_account.cpp
 * This file defines the VISHNU add local account command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "localAccountUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include "GenericCli.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddLocalAccountFunc {

  UMS_Data::LocalAccount mnewAcLogin;

  AddLocalAccountFunc(UMS_Data::LocalAccount newAcLogin): mnewAcLogin(newAcLogin)
  {};

  int operator()(std::string sessionKey) {
    string sshPublicKey;
    int res=addLocalAccount(sessionKey,mnewAcLogin,sshPublicKey);// call the UMS add local account service

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
    publicKeyName.append(mnewAcLogin.getUserId());
    publicKeyName.append("-"+mnewAcLogin.getMachineId());

    ofstream os(publicKeyName.c_str());
    os << sshPublicKey;

    std::cout << "The ssh public key path is  " << publicKeyName << std::endl;

    return res;
  }
};

int main (int ac, char* av[]){

  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::LocalAccount newAcLogin;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(newAcLogin),_1));
  boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(newAcLogin),_1));


  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,dietConfig,fMachineId,
      fAcLogin,fSshKeyPath,fHomeDirectory,1);

  CLICmd cmd = CLICmd (ac, av, opt);
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"userId machineId acLogin sshKeyPath homeDirectory");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"userId machineId acLogin sshKeyPath homeDirectory");
    return 0;
  }

  AddLocalAccountFunc apiFunc(newAcLogin);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main




