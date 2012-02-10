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

  UMS_Data::AuthSystem mnewAuthsystem;

  AddAuthenticationSystemFunc(const UMS_Data::AuthSystem& newAuthsystem ):
     mnewAuthsystem(newAuthsystem)
  {};

  int operator()(std::string sessionKey) {


     int res=addAuthSystem(sessionKey,mnewAuthsystem);

      cout <<"The new authentication system identifier is " << mnewAuthsystem.getAuthSystemId() << endl;

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::AuthSystem newAuthsystem;

  /******** Callback functions ******************/

  StringcallBackType fName( boost::bind(&UMS_Data::AuthSystem::setName,boost::ref(newAuthsystem),_1));

  StringcallBackType fURI( boost::bind(&UMS_Data::AuthSystem::setURI,boost::ref(newAuthsystem),_1));

  StringcallBackType fAuthLogin( boost::bind(&UMS_Data::AuthSystem::setAuthLogin,boost::ref(newAuthsystem),_1));

  StringcallBackType fAuthPassword(boost::bind(&UMS_Data::AuthSystem::setAuthPassword,boost::ref(newAuthsystem),_1));

 EncryptioncallBackType fUserPasswordEncryption( boost::bind(&UMS_Data::AuthSystem::setUserPasswordEncryption,boost::ref(newAuthsystem),_1));


  AuthcallBackType fType( boost::bind(&UMS_Data::AuthSystem::setType,boost::ref(newAuthsystem),_1));

  StringcallBackType fLdapBase(boost::bind(&UMS_Data::AuthSystem::setLdapBase,boost::ref(newAuthsystem),_1));


  /**************** Describe options *************/

boost::shared_ptr<Options> opt= makeAuthSystemOptions(av[0],dietConfig, fName,
fURI,fAuthLogin,fAuthPassword,fUserPasswordEncryption,fType,fLdapBase,1);


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

  AddAuthenticationSystemFunc apiFunc(newAuthsystem);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main

