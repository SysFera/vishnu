/**
 * \file get_file_info.cpp
 * This file defines the VISHNU  get file infos  command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "FMS_Data.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;


boost::shared_ptr<Options>
makeFMSCommonOpt(string pgName, string& path,
              string& dietConfig){
 
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);
  
  // All cli options
  
  opt->add("path,p",
      "represents the path of the file to display",
      HIDDEN,
      path,1);

  opt->setPosition("path",1);

  return opt;
}


int main (int ac, char* av[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string path;

  /********** EMF data ************/
  
  
  /******** Callback functions ******************/

     
  /*********** Out parameters *********************/

  FileStat fileStat;
  
  /**************** Describe options *************/
  
  boost::shared_ptr<Options> opt ( makeFMSCommonOpt( av[0], path, dietConfig )   );
  

  CLICmd cmd = CLICmd (ac, av, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt, " path");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options] path");
    return 0;
  }




  // Process command
  try {

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      errorUsage(av[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : getFileInfos
    if(false==sessionKey.empty()){
      cout <<currentSessionKeyMsg << sessionKey <<endl;

      getFilesInfo(sessionKey,path,fileStat);
      std::cout << "the file properties: \n";
      std::cout <<" path: " << fileStat.getPath() << "\n";
      std::cout <<" owner: " << fileStat.getOwner() << "\n";
      std::cout <<" group: " <<  fileStat.getGroup() << "\n";
      std::cout <<" uid: " <<  fileStat.getUid() << "\n";
      std::cout <<" gid: " <<  fileStat.getGid() << "\n";
      std::cout <<" size: " <<  fileStat.getSize() << "\n";
      std::cout <<" perms: " << oct << fileStat.getPerms() << "\n";
      boost::posix_time::ptime pt;
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getAtime()));
      //pt =  boost::posix_time::from_time_t(fileStat.getAtime());
      std::cout <<" atime : " << boost::posix_time::to_simple_string(pt) << "\n";
      pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getMtime()));
      std::cout <<" mtime : " << boost::posix_time::to_simple_string(pt) << "\n";
      time_t time = vishnu::convertUTCtimeINLocaltime(fileStat.getCtime());
      pt =  boost::posix_time::from_time_t(time);
      std::cout <<" ctime : " << boost::posix_time::to_simple_string(pt) << "\n";
    }
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(av[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return CLI_ERROR_RUNTIME;
  }
  return 0;
}
