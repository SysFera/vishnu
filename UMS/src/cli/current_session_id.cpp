/**
 * \file current_session_id.cpp
 * This file defines the VISHNU get current session_id command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include <unistd.h>                     // for getppid
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <exception>                    // for exception
#include <iostream>                     // for operator<<, basic_ostream, etc
#include <string>                       // for allocator, operator+, etc

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options
#include "UserException.hpp"            // for ERRCODE_CLI_ERROR_RUNTIME
#include "VishnuException.hpp"          // for VishnuException, VISHNU_OK
#include "cliUtil.hpp"                  // for errorUsage, ::EXECERROR
#include "sessionUtils.hpp"             // for checkBadSessionKeyError, etc


using namespace std;
using namespace vishnu;


int main (int ac, char* av[]) {
  /**************** Describe options *************/
  try{
    boost::shared_ptr< Options> opt(new Options(av[0]));

    bool isEmpty;
    //To process list options
    GenericCli().processListOpt(opt, isEmpty, ac, av);

    // get the sessionId
    std::string sessionId=getLastSessionId(getppid());

    // Vishnu call
    if (false == sessionId.empty()){
      std::cout << "current sessionId: " <<  sessionId << "\n";
      return VISHNU_OK;
    }

    errorUsage(av[0],"cannot retrieve sessionId");
    return ERRCODE_CLI_ERROR_RUNTIME;

  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return ERRCODE_CLI_ERROR_RUNTIME;
  }
}// end of main
