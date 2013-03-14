/**
 * \file close.cpp
 * This file defines the VISHNU close command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <unistd.h>                     // for getppid
#include <boost/smart_ptr/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "GenericCli.hpp"               // for GenericCli
#include "Options.hpp"                  // for Options, ::ENV
#include "api_ums.hpp"                  // for close
#include "sessionUtils.hpp"             // for getSessionLocation, etc

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


  string configFile;

  /**************** Describe options *************/
  boost::shared_ptr< Options> opt(new Options(av[0]) );


  opt->add("configFile,c",
          "VISHNU configuration file",
          ENV,
          configFile);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  CloseFunc apiFunc;
  return GenericCli().run(apiFunc, configFile, ac, av);

}// end of main
