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

struct RemoveDirFunc {

  std::string mpath;

RemoveDirFunc(const std::string& path):mpath(path){}

  int operator()(std::string sessionKey) {
    
    int res = removeDir(sessionKey,mpath);
    
    return res;
  }
};

int main (int argc, char* argv[]){
    /******* Parsed value containers ****************/
  string dietConfig;
  string path;
  
  //buid options and parse 
  
  ParseRemoteCommandOptions (argc,argv,dietConfig,path);
  RemoveDirFunc apiFunc(path);
  return GenericCli().run(apiFunc, dietConfig, argc, argv);

}
