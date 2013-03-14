/**
 * \file add_authentication_system.cpp
 * This file defines the VISHNU add authentication system command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind.hpp>
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <iostream>                     // for basic_ostream, operator<<, etc
#include <string>                       // for operator<<, string

#include "AuthSystem.hpp"               // for AuthSystem
#include "api_ums.hpp"                  // for addAuthSystem
#include "authenticationSystemUtils.hpp"
#include "GenericCli.hpp"

class Options;

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


int main (int ac, char* av[]) {
  /******* Parsed value containers ****************/
  string configFile;

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
  boost::shared_ptr<Options> opt= makeAuthSystemOptions(av[0], configFile, fName,
                                                        fURI, fAuthLogin,
                                                        fAuthPassword,
                                                        fUserPasswordEncryption,
                                                        fType, fLdapBase, 1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddAuthenticationSystemFunc apiFunc(newAuthsystem);

  return GenericCli().run(apiFunc, configFile, ac, av);
}  // end of main
