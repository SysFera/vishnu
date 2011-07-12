/**
 * \file remove_file.cpp
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

using namespace FMS_Data;

int main (int ac, char* av[]){
  

  /******* Parsed value containers ****************/
  string dietConfig;
  string path;

  /********** EMF data ************/
  FMS_Data::RmFileOptions rmFileOptions;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));
  
  opt->add("isRecursive,r",
      "It specifies when the remove command is recursive (case of directory) or not.",
      CONFIG);
  
  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");
 
  // Parse the cli and setting the options found

  if(opt->count("isRecursive")){
    rmFileOptions.setIsRecursive(true);
  }

  FileActionFunc<REMOVEFILE> apiFunc(path,rmFileOptions);
  return GenericCli().run(apiFunc, dietConfig, ac, av);

}
