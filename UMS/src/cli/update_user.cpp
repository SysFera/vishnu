/**
 * \file update_user.cpp
 * This file defines the VISHNU update user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/bind/arg.hpp>           // for arg
#include <boost/bind.hpp>          // for bind_t, bind
#include <boost/function.hpp>  // for function1
#include <boost/ref.hpp>                // for reference_wrapper, ref
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <sstream>                      // for basic_stringbuf<>::int_type, etc
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::CONFIG, ::HIDDEN
#include "User.hpp"                     // for User
#include "api_ums.hpp"                  // for updateUser
#include "userUtils.hpp"                // for makeUserOptions

using namespace std;
using namespace vishnu;

struct UpDateUserFunc {

  UMS_Data::User mupDateUser;

  UpDateUserFunc(UMS_Data::User upDateUser):
  mupDateUser(upDateUser)
  {};

  int operator()(std::string sessionKey) {
    int res = updateUser(sessionKey,mupDateUser);
    return res;
  }
};


int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string configFile;

  /********** EMF data ************/

  UMS_Data::User upUser;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::User::setUserId,boost::ref(upUser),_1));
  boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(upUser),_1));
  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::User::setStatus,boost::ref(upUser),_1));
  boost::function1<void,string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(upUser),_1));
  boost::function1<void,string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(upUser),_1));
  boost::function1<void,string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(upUser),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options>opt= makeUserOptions(av[0],configFile,fPrivilege,fFirstname, fLastname,fEmail);

  opt->add("status,s",
           "the status of the user: 0 (inactive) or 1 (active)",
           CONFIG,
           fStatus);

  opt->add("userId,u",
           "represents the VISHNU user identifier",
           HIDDEN,
           fUserId,
           1);


  opt->setPosition("userId", 1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  UpDateUserFunc upFunc(upUser);
  return GenericCli().run(upFunc, configFile, ac, av);
}// end of main
