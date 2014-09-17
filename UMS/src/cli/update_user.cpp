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

struct UpDateUserFunc {

  UMS_Data::User mupDateUser;

  UpDateUserFunc(UMS_Data::User upDateUser):
    mupDateUser(upDateUser)
  {};

  int operator()(std::string sessionKey) {
    int res = vishnu::updateUser(sessionKey,mupDateUser);
    return res;
  }
};


int main (int ac, char* av[]){

  std::string configFile;
  UMS_Data::User userInfo;
  boost::function1<void,std::string> fUserId( boost::bind(&UMS_Data::User::setUserId,boost::ref(userInfo),_1));
  boost::function1<void,UMS_Data::StatusType> fStatus( boost::bind(&UMS_Data::User::setStatus,boost::ref(userInfo),_1));
  boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(userInfo),_1));
  boost::function1<void,std::string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(userInfo),_1));
  boost::function1<void,std::string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(userInfo),_1));
  boost::function1<void,std::string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(userInfo),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options>opt= makeUserOptions(av[0],
      configFile,
      fUserId,
      fStatus,
      fPrivilege,
      fFirstname,
      fLastname,
      fEmail,
      0);

  bool isEmpty;
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  UpDateUserFunc upFunc(userInfo);
  return GenericCli().run(upFunc, configFile, ac, av);
}// end of main
