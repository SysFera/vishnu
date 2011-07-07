/**
 * \file copy_file.cpp
 * This file defines the VISHNU copy file command 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "fileTransferUtils.hpp"
#include "FMS_Data.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string src;
  string dest;

   /********** EMF data ************/
  FMS_Data::CpFileOptions cpFileOptions;


  copyParseOptions ( argc, argv,dietConfig,src,dest,cpFileOptions,CP);

  TransferSyncFunc<CP> apiFunc(src, dest, cpFileOptions);
 
  return GenericCli().run(apiFunc, dietConfig, argc, argv);
}



