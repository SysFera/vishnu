/**
 * \file move_async_file.cpp
 * This file defines the VISHNU move async file command 
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
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string src;
  string dest;
  string trCmdStr;
  int trCmd;

  /******* Output value ****************/
  FileTransfer transferInfo;

   /********** EMF data ************/
  FMS_Data::CpFileOptions cpFileOptions;

  /**************** Describe options *************/

  boost::shared_ptr<Options> opt(makeTransferCommandOptions(argv[0], dietConfig, trCmdStr, src, dest));

/*  CLICmd cmd = CLICmd (argc, argv, opt);

 // Parse the cli and setting the options found
  ret = cmd.parse(FMS_env_name_mapper());
 
  if(trCmdStr.size()!=0) {
    size_t pos = trCmdStr.find_first_not_of("0123456789");
    if(pos!=std::string::npos) {
      if(trCmdStr.compare("scp")==0 || trCmdStr.compare("SCP")==0){
        trCmd = 0;
      } else if(trCmdStr.compare("rsync")==0 || trCmdStr.compare("RSYNC")==0){
        trCmd = 1;
      } else {
        std::cerr << "Unknown file transfer command type " << trCmdStr << std::endl;
        return 0;
      }
    } else {
      trCmd = convertToInt(trCmdStr);
    }
    cpFileOptions.setTrCommand(trCmd);
  }
 */

ret=copyParseOptions ( opt, argc, argv,cpFileOptions );


  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] src dest");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  
  checkVishnuConfig(*opt);  
  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] src dest");
    return 0;
  }

  TransferAsyncFunc<MV> apiFunc(src, dest, transferInfo, cpFileOptions);
 
  return GenericCli().run(apiFunc, dietConfig, argc, argv);
}


