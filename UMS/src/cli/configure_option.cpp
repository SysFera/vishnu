#include "configureOptionUtils.hpp"
#include "utils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){



  boost::shared_ptr<Options> opt(new Options(av[0])) ;



  try {

    commonConfigure(opt,  ac,  av,&configureOption);
      
    printSuccessMessage();


  }

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt,"[options] optionName value ","required parameter is missing");
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;
  }

  catch(std::exception& e){

    errorUsage(av[0], e.what());
    return 1;
  }

  return 0;

}// end of main


