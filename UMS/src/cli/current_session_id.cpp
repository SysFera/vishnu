/**
 * \file current_session_id.cpp
 * This file defines the VISHNU get current session_id command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "GenericCli.hpp"
#include "UserException.hpp"
#ifdef __WIN32__
#include "OSIndependance.hpp"
#endif
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


int main (int ac, char* av[]){



  /**************** Describe options *************/
  try{
  boost::shared_ptr< Options> opt(new Options(av[0]) );

  // get the sessionId
#ifdef __WIN32__
  DWORD cpid = GetCurrentProcessId();
  std::string sessionId=getLastSessionId((int)GetParentProcessID(cpid));
#else
  std::string sessionId=getLastSessionId(getppid());
#endif

  // DIET call 
  if(false==sessionId.empty()){
    printSessionKeyMessage();
    std::cout << "current sessionId: " <<  sessionId << "\n";

      printSuccessMessage();
    return VISHNU_OK;

  }
  errorUsage(av[0],"cannot retrieve sessionId");
  return ERRCODE_CLI_ERROR_RUNTIME;

  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
#ifdef __WIN32__
      DWORD rcpid = GetCurrentProcessId();
      removeBadSessionKeyFromFile((int)GetParentProcessID(rcpid));
#else
      removeBadSessionKeyFromFile(getppid());
#endif      
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return ERRCODE_CLI_ERROR_RUNTIME;
  }


}// end of main



