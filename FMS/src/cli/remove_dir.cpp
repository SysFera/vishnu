/**
 * \file remove_dir.cpp
 * This file defines the VISHNU remove file command 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "cmdArgs.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"
#include "remoteCommandUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int argc, char* argv[]){
    /******* Parsed value containers ****************/
  string dietConfig;
  string path;
  
  //buid options and parse 
  ParseRemoteCommandOptions (argc,argv,dietConfig,path);
  
  FileActionFunc<REMOVEDIR> apiFunc(path);
  return GenericCli().run(apiFunc, dietConfig, argc, argv);

}
