/**
 * \file head.cpp
 * This file defines the VISHNU  head of file command 
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
#include "sessionUtils.hpp"
#include "cmdArgs.hpp"
#include "remoteCommandUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;


struct HeadOfFileFunc {

  std::string mpath;
  HeadOfFileOptions mhofOptions;
  HeadOfFileFunc(const std::string& path,const HeadOfFileOptions& hofOptions):mpath(path),mhofOptions(hofOptions){}

  int operator()(std::string sessionKey) {

    string contentOfHeadOfFile;

    int res=head(sessionKey,mpath,contentOfHeadOfFile, mhofOptions);
    cout << contentOfHeadOfFile << "\n"; 
    return res;
  }
};

int main (int ac, char* av[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string path;

  /********** EMF data ************/
  HeadOfFileOptions hofOptions;

  /******** Callback functions ******************/
  boost::function1<void,int> fNline(boost::bind(&FMS_Data::HeadOfFileOptions::setNline,boost::ref(hofOptions),_1));

  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));

  opt->add("nline,n",
      "The first line to display",
      CONFIG,
      fNline);

  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");
  HeadOfFileFunc apiFunc(path,hofOptions);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}
