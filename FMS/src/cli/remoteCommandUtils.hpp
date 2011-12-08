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
 * \brief  A define type
 */

typedef enum{
  CREATEFILE,
  CREATEDIR,
  REMOVEFILE,
  REMOVEDIR
} FileActionType;

struct NoOptions{};

template <FileActionType fileActionType,typename CommandOptionsType>
struct FileApiCommand {
};
template <>
struct FileApiCommand<CREATEFILE,NoOptions> {
static const int api (const std::string& sessionKey,const std::string& path,const NoOptions& options ){

return  createFile(sessionKey,path);

}
};
template <>
struct FileApiCommand<CREATEDIR,FMS_Data::CreateDirOptions> {
static const int api (const std::string& sessionKey,const std::string& path, const FMS_Data::CreateDirOptions& options ){

return  createDir(sessionKey,path,options);

}
};

template <>
struct FileApiCommand<REMOVEFILE,FMS_Data::RmFileOptions> {
static const int api (const std::string& sessionKey,const std::string& path, const FMS_Data::RmFileOptions& options ){

return  removeFile(sessionKey,path,options);

}
};

template <>
struct FileApiCommand<REMOVEDIR,NoOptions> {
static const int api (const std::string& sessionKey,const std::string& path, const NoOptions& options ){

return  removeDir(sessionKey,path);

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
   * \param path The path to treat
   * \brief Constructor
   */
  FileActionFunc(const std::string& path):mpath(path){}

  /**
   * \param path The path to treat
   * \param options The file transfer option
   * \brief Constructor
   */
  FileActionFunc(const std::string& path,const CommandOptionsType& options):mpath(path),moptions(options){}

  /**
   * \brief To execute the FileAction operation
   * \param sessionKey The identifier of the session
   * \return non-zero on error
   */
  int operator()(std::string sessionKey) {
    
     return FileApiCommand<fileActionType,CommandOptionsType>::api(sessionKey,mpath,moptions);
    
    /*int res = -1;
    switch(fileActionType) {
      case CREATEFILE:
        res = createFile(sessionKey,mpath);
        break;
      case CREATEDIR:
        res = createDir(sessionKey,mpath,moptions);
        break;
      case REMOVEFILE:
        res = removeFile(sessionKey,mpath,moptions);
        break;
      case REMOVEDIR:
        res = removeDir(sessionKey,mpath);
        break;
      default:
        break;
    }

    return res;

  */
  }
};




#endif
