#include "configureOptionUtils.hpp"
#include "utils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  boost::shared_ptr<Options> opt(new Options(av[0])) ;

// {{RELAX<CODEREDUCER> The error handling is the same in all command

  try {

    commonConfigure(opt,  ac,  av,&configureOption);
      
    printSuccessMessage();


  }

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] optionName value ",requiredParamMsg);

    return CLI_ERROR_MISSING_PARAMETER;
  }

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }


  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0], e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

  // }}RELAX<CODEREDUCER>

}// end of main


