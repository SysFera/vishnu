/**
 * \file more.cpp
 * This file defines the VISHNU  content of file command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "FMS_Data.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"
#include "remoteCommandUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;


struct ContentOfFileFunc {

  std::string mpath;
  ContentOfFileFunc(const std::string& path):mpath(path){}

  int operator()(std::string sessionKey) {

    string mfileContent;

      int res=more(sessionKey,mpath,mfileContent);
    cout <<mfileContent << "\n"; 
    return res;
  }
};



int main (int ac, char* av[]){
  

  /******* Parsed value containers ****************/
  string dietConfig;
  string path;

  
  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));
  
  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");
  ContentOfFileFunc apiFunc(path);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}
