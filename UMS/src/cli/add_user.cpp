/**
 * \file add_user.cpp
 * This file defines the VISHNU add user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "userUtils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
#include<boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct AddUserFunc {

  UMS_Data::User mnewUser;

  AddUserFunc(const UMS_Data::User& newUser ):
     mnewUser(newUser)
  {};

  int operator()(std::string sessionKey) {


     int res=addUser(sessionKey,mnewUser);

      cout <<"The user identifier is " << mnewUser.getUserId() << endl;

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string configFile;

  /********** EMF data ************/

  UMS_Data::User newUser;

  /******** Callback functions ******************/

  boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(newUser),_1));
  boost::function1<void,string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(newUser),_1));
  boost::function1<void,string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(newUser),_1));
  boost::function1<void,string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(newUser),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options>opt= makeUserOptions(av[0],configFile,fPrivilege,fFirstname, fLastname,fEmail,1);


  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  AddUserFunc apiFunc(newUser);
  return GenericCli().run(apiFunc, configFile, ac, av);
}  // end of main
