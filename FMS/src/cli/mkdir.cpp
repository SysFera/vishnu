/**
 * \file mkdir.cpp
 * This file defines the VISHNU create directory command 
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
using namespace FMS_Data;

int main (int ac, char* av[]){
  
 /******* Parsed value containers ****************/
  string dietConfig;
  string path;

  /********** EMF data ************/
  FMS_Data::CreateDirOptions mkdirOptions;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));

  opt->add("isRecursive,p",
      "It specifies when the make directory command is recursive (make the parent directory if needed) or not.",
      CONFIG);
  
  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");
 
  // Parse the cli and setting the options found

  if(opt->count("isRecursive")){
    mkdirOptions.setIsRecursive(true);
  } 
  
  FileActionFunc<CREATEDIR,FMS_Data::CreateDirOptions> apiFunc(path,mkdirOptions);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}
