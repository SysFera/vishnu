/**
 * \file update_user.cpp
 * This file defines the VISHNU update user command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "utils.hpp"
#include "userUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string dietConfig;

  string sessionKey;

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


  try {
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);


    opt->parse_env(env_name_mapper());


    opt->notify();

    /********  Process **************************/


    checkVishnuConfig(*opt);
    
    /************** Call UMS connect service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }


    // get the session key

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout << "the current session key is " << sessionKey <<endl; 
      
      updateUser(sessionKey,upUser);
     
      printSuccessMessage();
    }

  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing


    usage(*opt," userId ","required parameter is missing");
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


