/**
 * \file list_file_transfers.cpp
 * This file defines the VISHNU list file transfers command
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "FMS_Data.hpp"
#include <boost/bind.hpp>
#include "FMSDisplayer.hpp"
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

/**
 * \brief To build options for the VISHNU list directory of file command
 * \param pgName : The name of the command
 * \param dietConfig: Represents the VISHNU config file
 * \param ftransferId: The file transfer identifier
 * \param ffromMachineId: The machine that is the source of the file transfer
 * \param fuserId: The user identifier
 * \param fstatus: The file transfer status
 */
boost::shared_ptr<Options>
makeListFileTransferTrOpt(string pgName,
    string& dietConfig,
    boost::function1<void, string>& ftransferId,
    boost::function1<void, string>& ffromMachineId,
    boost::function1<void, string>& fuserId,
    string& statusStr){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("transferId,t",
      "A given transfer id",
      CONFIG,
      ftransferId);

 opt->add("fromMachineId,m",
      "The machine that is the source of the file transfer",
      CONFIG,
      ffromMachineId);

 opt->add("userId,u",
      "Allows an admin to stop file transfers of a specific user",
      CONFIG,
      fuserId);

 opt->add("status,s",
      "The file transfer status. The different  available status are:\n"
      "0 or I: for INPROGRESS file transfer\n"
      "1 or T: for COMPLETED file transfer\n"
      "2 or C: for CANCELLED file transfer\n"
      "3 or F: for FAILED file transfer\n",
      CONFIG,
      statusStr);

  return opt;
}



// List file transfer functor
struct ListFileTransferFunc {

  FMS_Data::LsTransferOptions mlsFileTransferOptions;



  ListFileTransferFunc(const FMS_Data::LsTransferOptions& lsFileTransferOptions):mlsFileTransferOptions(lsFileTransferOptions)
  {};

  int operator()(std::string sessionKey) {

    FileTransferList fileTransferList;

    int res=listFileTransfers(sessionKey, fileTransferList,mlsFileTransferOptions);

    //To display the file transfer list
    std::cout << fileTransferList << std::endl;

    return res;
  }
};



int main (int ac, char* av[]) {
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string statusStr;

   /********** EMF data ************/
  FMS_Data::LsTransferOptions lsFileTransferOptions;

  /******** Callback functions ******************/
  boost::function1<void, string> ftranferId(boost::bind(&FMS_Data::LsTransferOptions::setTransferId, boost::ref(lsFileTransferOptions),_1));
  boost::function1<void, string> ffromMachineId(boost::bind(&FMS_Data::LsTransferOptions::setFromMachineId, boost::ref(lsFileTransferOptions),_1));
  boost::function1<void, string> fuserId(boost::bind(&FMS_Data::LsTransferOptions::setUserId, boost::ref(lsFileTransferOptions),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt= makeListFileTransferTrOpt(av[0], dietConfig, ftranferId, ffromMachineId, fuserId, statusStr);

  CLICmd cmd = CLICmd (ac, av, opt);

 // Parse the cli and setting the options found
    ret = cmd.parse(env_name_mapper());


  if (statusStr.size() != 0) {
    int status;
    size_t pos = statusStr.find_first_not_of("0123456789");

    if (pos != std::string::npos) {
      if (statusStr.size() == 1) {
        switch(statusStr[0]) {
          case 'I' :
            status = 0;
            break;
          case 'T' :
            status = 1;
            break;
          case 'C' :
            status = 2;
            break;
          case 'F' :
            status = 3;
            break;
          default:
            status = -1;
            break;
        }
      }
      if ((statusStr.size() > 1) || (status == -1)) {
        std::cerr << "Unknown file transfer status " << statusStr << std::endl;
        return 0;
      }
    } else {
      status = convertToInt(statusStr);
    }
    lsFileTransferOptions.setStatus(status);
  }

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[options] ");
    return 0;
  }

  // Process command
 ListFileTransferFunc apiFunc( lsFileTransferOptions);
 return GenericCli().run(apiFunc, dietConfig, ac, av);

}
