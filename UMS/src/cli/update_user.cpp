/**
 * \file update_user.cpp
 * This file defines the VISHNU update user command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "userUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

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

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::User upUser;

  /******** Callback functions ******************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::User::setUserId,boost::ref(upUser),_1));
  boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(upUser),_1));
  boost::function1<void,string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(upUser),_1));
  boost::function1<void,string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(upUser),_1));
  boost::function1<void,string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(upUser),_1));

  /**************** Describe options *************/

  boost::shared_ptr<Options>opt= makeUserOptions(av[0],dietConfig,fPrivilege,fFirstname, fLastname,fEmail);



  opt->add("userId,u",
           "represents the VISHNU user identifier",
           HIDDEN,
           fUserId,
           1);


  opt->setPosition("userId",-1);

  CLICmd cmd = CLICmd (ac, av, opt);
   // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"UserId");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"UserId");
    return 0;
  }

  UpDateUserFunc upFunc(upUser);
  return GenericCli().run(upFunc, dietConfig, ac, av);


}// end of main


