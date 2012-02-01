/**
 * \file add_authentication_system.cpp
 * This file defines the VISHNU add authentication system command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "authenticationSystemUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddAuthenticationSystemFunc {

  UMS_Data::AuthSystems mnewAuthsystem;

  AddAuthenticationSystemFunc(const UMS_Data::AuthSystems& newAuthsystem ):
     mnewAuthsystem(newAuthsystem)
  {};

  int operator()(std::string sessionKey) {


     int res=addAuthenticationSystem(sessionKey,mnewAuthsystem);
    
      cout <<"The new authentication system identifier is " << mnewAuthsystem.getAuthSystemId() << endl;

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::AuthSystems newAuthsystem;
  
  std::string ldapBase;
  /******** Callback functions ******************/

  StringcallBackType fName( boost::bind(&UMS_Data::AuthSystems::setName,boost::ref(newAuthsystem),_1));
  
  StringcallBackType fURI( boost::bind(&UMS_Data::AuthSystems::setURI,boost::ref(newAuthsystem),_1));
  
  StringcallBackType fAuthLogin( boost::bind(&UMS_Data::AuthSystems::setAuthLogin,boost::ref(newAuthsystem),_1));
 
  StringcallBackType fAuthPassword(boost::bind(&UMS_Data::AuthSystems::setAuthPassword,boost::ref(newAuthsystem),_1)); 
  
 EncryptioncallBackType fUserPasswordEncryption( boost::bind(&UMS_Data::AuthSystems::setUserPasswordEncryption,boost::ref(newAuthsystem),_1));


  AuthcallBackType fType( boost::bind(&UMS_Data::AuthSystems::setType,boost::ref(newAuthsystem),_1));



  /**************** Describe options *************/

boost::shared_ptr<Options> opt= makeAuthSystemOptions(av[0],dietConfig, fName,
fURI,fAuthLogin,fAuthPassword,fUserPasswordEncryption,fType,ldapBase,1);
  

CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"name URI authLogin authPassword userPasswordEncryption type");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"name URI authLogin authPassword userPasswordEncryption type");
    return 0;
  }

if(opt->count ("ldapBase")){

  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
 UMS_Data::AuthSystemsOptions_ptr options_ptr = ecoreFactory->createAuthSystemsOptions();
options_ptr->setLdapBase(ldapBase);
newAuthsystem.setOptions(options_ptr);



}


  AddAuthenticationSystemFunc apiFunc(newAuthsystem);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main

