/**
 * \file remoteCommandUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU remote
 * commands
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */



#ifndef REMOTECOMMANDUTILS_HH
#define REMOTECOMMANDUTILS_HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "FMS_Data.hpp"

#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "cmdArgs.hpp"
#include "CLICmd.hpp"
#include "GenericCli.hpp"
class Options;

using namespace std;
using namespace vishnu;


void ParseRemoteCommandOptions (int argc, char* argv[],std::string& dietConfig, std::string& path){

// build remote command options

boost::shared_ptr<Options> opt=processOpt(argv[0], dietConfig);
  
opt->add("path,p",
      "The file following the pattern [host:]file path.",
      HIDDEN,
      path,1);
  opt->setPosition("path",1);

bool isEmpty;
GenericCli().processListOpt( opt, isEmpty,argc,argv,"path");



}
#endif
