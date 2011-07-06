/**
 * \file close.cpp
 * This file defines the VISHNU close command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct CloseFunc {


  int operator()(std::string sessionKey) {
    
    int res = close(sessionKey);
      
    std::string sessionFile=getSessionLocation(getppid());
      
    removeLastSession(sessionFile);
    
    return res;
  }
};


int main (int ac, char* av[]){


  string dietConfig;

  /**************** Describe options *************/


  boost::shared_ptr< Options> opt(new Options(av[0]) );


  opt->add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options]");
    return 0;
  }
 
  CloseFunc apiFunc;
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}// end of main



