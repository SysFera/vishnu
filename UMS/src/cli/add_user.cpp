/**
 * \file add_user.cpp
 * This file defines the VISHNU add user command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "userUtils.hpp"
#include "sessionUtils.hpp"
#include<boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  string sessionKey;

  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF data ************/

  UMS_Data::User newUser;

  /******** Callback functions ******************/

  boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(newUser),_1));
  boost::function1<void,string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(newUser),_1));
  boost::function1<void,string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(newUser),_1));
  boost::function1<void,string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(newUser),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options>opt= makeUserOptions(av[0],dietConfig,fPrivilege,fFirstname, fLastname,fEmail,1);


  try{
    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/


    checkVishnuConfig(*opt);



    /************** Call UMS add user service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      
      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      addUser(sessionKey,newUser);
    
      cout <<"The user identifier is " << newUser.getUserId() << endl;
      
      printSuccessMessage();

    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," firstname lastname privilege email ",requiredParamMsg);
   
    return CLI_ERROR_MISSING_PARAMETER;
  }

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error
   
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]"; 

    errorUsage(av[0], msg, EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main

