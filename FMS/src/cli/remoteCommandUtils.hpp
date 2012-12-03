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

/**
 * \brief To build options for the VISHNU command line
 * \param pgName : The name of the command
 * \param dietConfig Represents the VISHNU config file
 * \param path  Represents the path of the file
 * \return  The built command
 */
boost::shared_ptr<Options>
makeRemoteCommandOpt(string pgName,
           string& dietConfig,
           string& path){


  boost::shared_ptr<Options> opt=processOpt(pgName, dietConfig);
  
  opt->add("path",
      "represents the path of the file",
      HIDDEN,
      path,1);

  opt->setPosition("path",1);


  return opt;
}


/**
 * \brief A generic functio to parse remote command involving a file 
 * \param argc The number of command line arguments
 * \param argv the command line arguments
 * \param dietConfig the diet client config file
 * \param path the path of the file 
 */
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

/**
 * \brief  File action type representation
 */

typedef enum{
  CREATEFILE,
  CREATEDIR,
  REMOVEFILE,
  REMOVEDIR
} FileActionType;

/**
 * \brief Represents non typed Options
 */ 
struct NoOptions{};

/**
 * \brief Generic type to represent function acting on file
 */
template <FileActionType fileActionType,typename CommandOptionsType>
struct FileApiCommand {
};

/**
 * \brief A specialization of FileApiCommand for file creation function 
 */

template <>
struct FileApiCommand<CREATEFILE,NoOptions> {
  /**
   * \brief create file function
   * \param sessionKey The session key
   * \param path the new directory path
   * \param options the file creation options
   * \return VISHNU_OK if the remote command succeeds or an error code otherwise
   */
static const int api (const std::string& sessionKey,const std::string& path,const NoOptions& options ){

return  touch(sessionKey,path);

}
};


/**
 * \brief A specialization of FileApiCommand for directory creation function 
 */

template <>
struct FileApiCommand<CREATEDIR,FMS_Data::CreateDirOptions> {
  /**
   * \brief create directory function
   * \param sessionKey The session key
   * \param path the new directory path
   * \param options the directory creation options
   * \return VISHNU_OK if the remote command succeeds or an error code otherwise
   */
static const int api (const std::string& sessionKey,const std::string& path, const FMS_Data::CreateDirOptions& options ){

return  mkdir(sessionKey,path,options);

}
};

/**
 * \brief A specialization of FileApiCommand for file deletion function 
 */
template <>
struct FileApiCommand<REMOVEFILE,FMS_Data::RmFileOptions> {
  /**
   * \brief remove file function
   * \param sessionKey The session key
   * \param path the file path
   * \param options the file deletion options
   * \return VISHNU_OK if the remote command succeeds or an error code otherwise
   */
static const int api (const std::string& sessionKey,const std::string& path, const FMS_Data::RmFileOptions& options ){

return  removeFile(sessionKey,path,options);

}
};

/**
 * \brief A specialization of FileApiCommand for file deletion function 
 */
template <>
struct FileApiCommand<REMOVEDIR,NoOptions> {
  /**
   * \brief remove directory function
   * \param sessionKey The session key
   * \param path the  directory path
   * \param options the directory deletion options
   * \return VISHNU_OK if the remote command succeeds or an error code otherwise
   */
static const int api (const std::string& sessionKey,const std::string& path, const NoOptions& options ){

return  rmdir(sessionKey,path);

}
};


/**
 * \brief A functor to handle file action (create/remove) api function
 */
template<FileActionType fileActionType, typename CommandOptionsType=NoOptions>
struct FileActionFunc {
  /**
   * \brief The path to treat 
   */
  std::string mpath;
  /**
   * \brief The file transfer options 
   */
  CommandOptionsType moptions;

  /**
   * \brief Constructor
   * \param path The path to treat
   */
  FileActionFunc(const std::string& path):mpath(path){}

  /**
   * \brief Constructor
   * \param path The path to treat
   * \param options The file transfer option
   */
  FileActionFunc(const std::string& path,const CommandOptionsType& options):mpath(path),moptions(options){}

  /**
   * \brief To execute the FileAction operation
   * \param sessionKey The identifier of the session
   * \return non-zero on error
   */
  int operator()(std::string sessionKey) {
    
     return FileApiCommand<fileActionType,CommandOptionsType>::api(sessionKey,mpath,moptions);
    
  }
};


#endif
