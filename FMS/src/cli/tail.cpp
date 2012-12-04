/**
 * \file tail.cpp
 * This file defines the VISHNU  tail of file command 
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
#include "cmdArgs.hpp"
#include "remoteCommandUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

struct TailOfFileFunc {

  std::string mpath;
  TailOfFileOptions mtofOptions;
  TailOfFileFunc(const std::string& path,const TailOfFileOptions& tofOptions):mpath(path),mtofOptions(tofOptions){}

  int operator()(std::string sessionKey) {

    string contentOfTailOfFile;

    int res=tail(sessionKey,mpath,contentOfTailOfFile, mtofOptions);
    cout << contentOfTailOfFile << "\n"; 
    return res;
  }
};


int main (int ac, char* av[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string path;

  /********** EMF data ************/
  TailOfFileOptions tofOptions;
  
  /******** Callback functions ******************/
  boost::function1<void,int> fNline(boost::bind(&FMS_Data::TailOfFileOptions::setNline,boost::ref(tofOptions),_1));
  
  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));
 
  opt->add("nline,n",
      "The last lines to display",
      CONFIG,
      fNline);

  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");
  TailOfFileFunc apiFunc(path,tofOptions);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}
