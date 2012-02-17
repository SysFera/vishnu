/**
 * \file connect_m.cpp
 * This file defines the VISHNU connect command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <stdlib.h>
#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "daemon_cleaner.hpp"
#include "utilVishnu.hpp"
#include "UMSVishnuException.hpp"
#include "UserException.hpp"

using namespace std;
using namespace vishnu;


bool isHelp(string s){
  return (s.size()>1 && s.at(0)=='-' && s.at(1)=='h');
}

bool isSubstitute(string s){
  return (s.size()>1 && s.at(0)=='-' && s.at(1)=='s');
}

bool isPolicy(string s){
  return (s.size()>1 && s.at(0)=='-' && s.at(1)=='p');
}

bool isDelay(string s){
  return (s.size()>1 && s.at(0)=='-' && s.at(1)=='d');
}



int main (int ac, char* av[]){

  int counter;

  /******* Parsed value containers ****************/

  string dietConfig;

  /********** EMF Data ****************************/

  UMS_Data::ConnectOptions connectOpt;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();

  /*********** In parameters **********************/

  UMS_Data::ListUsers listUsers;

  /*********** Out parameters *********************/
  std::string userId;
  boost::function1<void,UMS_Data::SessionCloseType> fClosePolicy( boost::bind(&UMS_Data::ConnectOptions::setClosePolicy,boost::ref(connectOpt),_1));
  boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ConnectOptions::setSessionInactivityDelay,boost::ref(connectOpt),_1));
  boost::function1<void,string> fSubstituteUserId( boost::bind(&UMS_Data::ConnectOptions::setSubstituteUserId,boost::ref(connectOpt),_1));

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,0,dietConfig,CONFIG);
  opt->add("sessionInactivityDelay,d", "The session inactivity delay",CONFIG,fSessionInactivityDelay);


  opt->add("substituteUserId,s","The substitute user identifier",CONFIG,fSubstituteUserId);

  opt->add("closePolicy,p","for closing session automatically",ENV,fClosePolicy );


  UMS_Data::Session session;

  dietConfig = string(getenv("VISHNU_CONFIG_FILE"));


  /********* Get all the couples user/pwd ******************/
  counter = 1;
  while (counter<ac) {
    string tmp(av[counter]);
    try {
// Checking token
      if (isPolicy(tmp)) {
        if (tmp.size()>2) {
          counter++;
          UMS_Data::SessionCloseType type(atoi(tmp.substr(2).c_str()));
          connectOpt.setClosePolicy(type);
        } else {
          UMS_Data::SessionCloseType type(atoi(av[counter+1]));
          connectOpt.setClosePolicy(type);
          counter +=2;
        }
      } // end if close policy
      else if (isDelay(tmp)) {
        if (tmp.size()>2) {
          counter++;
          connectOpt.setSessionInactivityDelay(atoi(tmp.substr(2).c_str()));
        } else {
          connectOpt.setSessionInactivityDelay(atoi(av[counter+1]));
          counter +=2;
        }
      } // end if delay
      else if (isSubstitute(tmp)) {
        if (tmp.size()>2) {
          counter++;
          connectOpt.setSubstituteUserId(tmp.substr(2));
        } else {
          connectOpt.setSubstituteUserId(string(av[counter+1]));
          counter +=2;
        }
      } // end if substitute
      else if (isHelp(tmp)){
        counter++;
        helpUsage(*opt,"[option] ");
        return 0;
      } else { // is a couple userid followed by password
        // If the second component isn't present
        if (counter+1>=ac) {
          helpUsage(*opt,"[option] ");
          return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
        }
        UMS_Data::User_ptr user = ecoreFactory->createUser();
        user->setUserId(tmp);
        user->setPassword(string(av[counter+1]));
        listUsers.getUsers().push_back(user);
        counter +=2 ;
      }
    } catch (std::out_of_range &e){
    }


  }// End while


    /************** Call UMS connect service *******************************/
  try{

    cleaner(const_cast<char*>(dietConfig.c_str()), ac, av);// lauch the daemon cleaner if it is not already running

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      errorUsage(av[0],dietErrorMsg,EXECERROR);

      return  ERRCODE_CLI_ERROR_DIET ;
    }

    connect(listUsers, session, connectOpt);// call the api extern connect service to get a session key

    vishnuFinalize();

    storeLastSession(session,getppid()); // store sessionKey into $HOME/.vishnu/sessions
    std::cout << "sessionId: " << session.getSessionId() << "\n";

    printSuccessMessage();


  }// End of try bloc


//  catch(po::required_option& e){// a required parameter is missing
//
//    usage(*opt,"[options] ",requiredParamMsg);
//
//    return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
//  }
//  // {{RELAX<CODEREDUCER> The error handling is the same in all command
//
//  catch(po::error& e){ // catch all other bad parameter errors
//    errorUsage(av[0], e.what());
//
//    return ERRCODE_INVALID_PARAM;
//  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    // handle the temporary password
    if(ERRCODE_TEMPORARY_PASSWORD==e.getMsgI()){

      errorUsage(av[0],"Your password is temporary, use vishnu_change_password command to change it",EXECERROR);
    }

    else{

      std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

      errorUsage(av[0],msg,EXECERROR);
    }

    return e.getMsgI() ;

  }

  catch(std::exception& e){ // catch all std runtime error

    errorUsage(av[0], e.what());

    return ERRCODE_CLI_ERROR_RUNTIME;
  }

  return 0;

// }}RELAX<CODEREDUCER>

}// end of main


