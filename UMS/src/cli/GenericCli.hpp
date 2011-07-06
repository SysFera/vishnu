
#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

class GenericCli {

  public:

    template <class ApiFunc>
    int run(ApiFunc function, std::string dietConfig, int ac, char*  av[]) {

      try{

        // initializing DIET

        if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

          errorUsage(av[0],dietErrorMsg,EXECERROR);

          return  CLI_ERROR_DIET ;
        }

        // get the sessionKey
        std::string sessionKey=getLastSessionKey(getppid());

        // DIET call 
        if(false==sessionKey.empty()){
          printSessionKeyMessage();
          //call of the api fuction
          function(sessionKey);
      
          printSuccessMessage(); 

        }
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
        return CLI_ERROR_RUNTIME;
      }

    }

};
