/**
 * \file list_dir.cpp
 * This file defines the VISHNU list dir command 
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
#include "cmdArgs.hpp"
#include <boost/bind.hpp>
#include "FMSDisplayer.hpp"
#include "GenericCli.hpp"
#include "remoteCommandUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

struct ListDirFunc {

  std::string mpath;
  LsDirOptions moptions;

  ListDirFunc(const std::string& path,const LsDirOptions& options):mpath(path),moptions(options){}

  int operator()(std::string sessionKey) {

    DirEntryList dirContent;

    int res= listDir(sessionKey, mpath, dirContent, moptions);

    if (false ==moptions.isLongFormat()) {

      for(unsigned int i = 0; i < dirContent.getDirEntries().size(); i++) {

        std::cout <<  (dirContent.getDirEntries().get(i))->getPath() <<"\n";
      } 

    }else{

      cout << dirContent << "\n"; 
    }
    return res;
  }
};


int main (int ac, char* av[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string path;
   
  /********** EMF data ************/
  FMS_Data::LsDirOptions lsDirOptions;
  
  /**************** Describe options *************/


  boost::shared_ptr<Options> opt(makeRemoteCommandOpt(av[0],dietConfig,path));

 opt->add("longFormat,l",
      "It specifies the long display format (all available file information",
      CONFIG);

  opt->add("allFiles,a",
      "Allows to display all files including hidden file",
      CONFIG);


  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,ac,av," path");


 if ( opt->count("allFiles")){
     lsDirOptions.setAllFiles(true);
  }

 if ( opt->count("longFormat")){
     lsDirOptions.setLongFormat(true);
  }

 ListDirFunc apiFunc(path,lsDirOptions);
  
 return GenericCli().run(apiFunc, dietConfig, ac, av);

}
