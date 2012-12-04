/**
 * \file fileTransferUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU file
 * transfer
 * command
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */



#ifndef FILETRANSFERUTILS_HH
#define FILETRANSFERUTILS_HH


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
class Options;

using namespace std;
using namespace vishnu;



/**
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */
struct FMS_env_name_mapper{

  /**
   * Mapper operator for environnement variable
   * \param name The environment variable name
   * \return The name used in the program
   */

  std::string operator ()(std::string name){

    std::string result= "";

    if (name=="VISHNU_CONFIG_FILE") {

      result="dietConfig";
    }

    if (name=="VISHNU_TRANSFER_CMD") {

      result="trCommand";
    }

    return result;
  }
};




/**
 * \brief To build options for the VISHNU copy of file command
 * \param pgName The name of the command
 * \param dietConfig Represents the VISHNU config file
 * \param trCmdStr The command to use to perform file transfer
 * \param src The source file to copy following the pattern [host:]file path
 * \param dest The path of the destination file
 * \return The built option
 */
boost::shared_ptr<Options>
makeTransferCommandOptions(string pgName,
    string& dietConfig,
    string& trCmdStr,
    string& src,
    string& dest){

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
      "The diet config file",
      ENV,
      dietConfig);

  opt->add("trCommand,t",
      "The command to use to perform file transfer. The different values  are:\n"
      "O or scp: for SCP transfer\n"
      "1 or rsync: for RSYNC transfer\n",
      ENV,
      trCmdStr);

  opt->add("src,s",
      "The source file to copy following the pattern [host:]file path.",
      HIDDEN,
      src,1);
  opt->setPosition("src",1);

  opt->add("dest,d",
      "The path of the destination file.",
      HIDDEN,
      dest,1);
  opt->setPosition("dest",1);

  return opt;
}

/**
 * \brief  A file transfer type
 */

typedef enum{
  MV,
  CP
}TransferType;


/**
 * \brief common parse function for file transfer command
 * \param argc The number of command line arguments
 * \param argv The command line arguments
 * \param dietConfig The VISHNU client config file
 * \param src The file transfer source
 * \param dest The file transfer destination
 * \param cpFileOptions The file transfer options
 * \param transferType The file transfer type
 */

void copyParseOptions (int argc, char* argv[],std::string& dietConfig,
    std::string& src, std::string& dest, FMS_Data::CpFileOptions& cpFileOptions, TransferType transferType){

  int ret; // Return value

  string trCmdStr;

  int trCmd;

  // build transfer command options

  boost::shared_ptr<Options> opt(makeTransferCommandOptions(argv[0], dietConfig, trCmdStr, src, dest));

  if (transferType==CP){

    opt->add("isRecursive,r",
        "It specifies when the copy is recursive (case of directory) or not.",
        CONFIG);
  }

  CLICmd cmd (argc, argv, opt);

  // Parse the cli and setting the options found
  ret = cmd.parse(FMS_env_name_mapper());

  if ( opt->count("help")){
    helpUsage(*opt,"[options] src dest");
    exit(0);
  }



  // Check for vishnu config file

  checkVishnuConfig(*opt);


  if(trCmdStr.size()!=0) {
    size_t pos = trCmdStr.find_first_not_of("0123456789");
    if(pos!=std::string::npos) {
      if(trCmdStr.compare("scp")==0 || trCmdStr.compare("SCP")==0){
        trCmd = 0;
      } else if(trCmdStr.compare("rsync")==0 || trCmdStr.compare("RSYNC")==0){
        trCmd = 1;
      } else {

        errorUsage (argv[0],"Unknown file transfer command type "+trCmdStr,PARAMERROR);
        exit (CLI_ERROR_INVALID_PARAMETER);
      }
    } else {
      trCmd = convertToInt(trCmdStr);
    }
    cpFileOptions.setTrCommand(trCmd);
  }


  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options] src dest");
    exit (ret);
  }

  if (opt->count("isRecursive")){
    cpFileOptions.setIsRecursive(true);
  }

}






/**
 * \brief A functor to handle asynchronous file transfer api function
 */

template<TransferType transferType>
struct TransferAsyncFunc {

  /**
   * \brief The file transfer source
   */
  std::string msrc;

  /**
   * \brief The file transfer destination
   */

  std::string mdest;
  /**
   * \brief The file transfer information
   */
  FMS_Data::FileTransfer mtransferInfo;

  /**
   * \brief The file transfer command options
   */

  FMS_Data::CpFileOptions mcpFileOptions;

  /**
   * \brief Constructor with parameters
   * \param src The source
   * \param dest The destination
   * \param transferInfo The file transfer complete information
   * \param cpFileOptions The file transfer options
   */
  TransferAsyncFunc(const std::string& src,const std::string& dest,const FMS_Data::FileTransfer& transferInfo,const FMS_Data::CpFileOptions& cpFileOptions ):msrc(src),mdest(dest), mtransferInfo(transferInfo), mcpFileOptions (cpFileOptions) {};

  /**
   * \brief () operator
   * \param sessionKey The session key
   * \return 0 if it succeeds or an error code otherwise
   */
  int operator()(const std::string& sessionKey) {
    int res;
    if (transferType==MV){

      res=amv(sessionKey, msrc, mdest, mtransferInfo, mcpFileOptions);

    }else {


      res=acp(sessionKey, msrc, mdest, mtransferInfo, mcpFileOptions);
    }

    std::cout << "The transfer identifier is " << mtransferInfo.getTransferId() << std::endl;

    return res;
  }
};



/**
 * \brief A functor to handle synchronous file transfer api function
 */

template<TransferType transferType>
struct TransferSyncFunc {
  /**
   * \brief The file transfer source
   */
  std::string msrc;
  /**
   * \brief The file transfer destination
   */
  std::string mdest;
  /**
   * \brief The file transfer options
   */
  FMS_Data::CpFileOptions mcpFileOptions;

  /**
   * \brief Constructor with parameters
   * \param src The source
   * \param dest The destination
   * \param cpFileOptions The file transfer options
   */


  TransferSyncFunc(const std::string& src,const std::string& dest,const FMS_Data::CpFileOptions& cpFileOptions ):msrc(src),mdest(dest),mcpFileOptions (cpFileOptions) {};

  /**
   * \brief () operator
   * \param sessionKey The session key
   * \return 0 if it succeeds or an error code otherwise
   */

  int operator()(const std::string& sessionKey) {
    int res;
    if (transferType==MV){

      res=mv(sessionKey, msrc, mdest, mcpFileOptions);

    }else {

      res=cp(sessionKey, msrc, mdest, mcpFileOptions);
    }

    return res;
  }
};


#endif



