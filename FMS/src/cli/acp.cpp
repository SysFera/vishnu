/**
 * \file acp.cpp
 * This file defines the VISHNU copy async file command
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
#include "FMSDisplayer.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;



int main (int argc, char* argv[]){


  /******* Parsed value containers ****************/
  string configFile;
  string src;
  string dest;

  /************* Output value *********************/
  FMS_Data::FileTransfer transferInfo;

   /********** EMF data ************/
  FMS_Data::CpFileOptions cpFileOptions;


  copyParseOptions ( argc, argv,configFile,src,dest,cpFileOptions,CP);

  TransferAsyncFunc<CP> apiFunc(src, dest, transferInfo, cpFileOptions);

  return GenericCli().run(apiFunc, configFile, argc, argv);
}
