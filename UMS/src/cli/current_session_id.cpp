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

int main (int ac, char* av[]) {

  try {
    std::string configFile;
    boost::shared_ptr<Options> opt(new Options(av[0]));

    opt->add("configFile,c",
             "VISHNU configuration file",
             ENV,
             configFile);

    bool isEmpty;
    GenericCli().processListOpt(opt, isEmpty, ac, av);

    std::string sessionId = getLastSessionId(getppid());
    if (! sessionId.empty()){
      std::cout << "Session ID : " <<  sessionId << "\n";
      return 0;
    }

    errorUsage(av[0], "cannot retrieve sessionId");
    return ERRCODE_CLI_ERROR_RUNTIME;

  } catch(VishnuException& ex){// catch all Vishnu runtime error
    std::string  msg = ex.getMsg()+" ["+ex.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    if (checkBadSessionKeyError(ex)){
      removeBadSessionKeyFromFile(getppid());
    }
    return ex.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return ERRCODE_CLI_ERROR_RUNTIME;
  }

  return 0;
}
