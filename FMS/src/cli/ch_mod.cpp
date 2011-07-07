/**
 * \file ch_mod.cpp
 * This file defines the VISHNU ch_mod command 
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
#include <sys/types.h>
#include "FMSDisplayer.hpp"
#include "GenericCli.hpp"
namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ChangeFileModeFunc {

  std::string mpath;
  mode_t  mmode;
  
  ChangeFileModeFunc(const std::string& path,const mode_t&  mode):mpath(path),mmode(mode){}

  int operator()(std::string sessionKey) {
    
      int res =chMod(sessionKey, mmode, mpath);
    return res;
  }
};





int main (int argc, char* argv[]){
  
  int ret; // Return value
 // ChModOptions chOptions;

  /******* Parsed value containers ****************/
  string dietConfig;
  string path;
  mode_t  mode;
  /******** Callback functions ******************/
  //boost::function1<void,string> fmode(boost::bind(&FMS_Data::ChModOptions::setMode,boost::ref(chOptions),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=processOpt(argv[0], dietConfig);

  opt->add("mode,m",
      "The acces rights of file/directory in octal sytem",
      HIDDEN,
      mode,1);
  opt->setPosition("mode",1); 

 opt->add("path,p",
      "The file/directory following the pattern [host:]file path",
      HIDDEN,
      path,1);
  opt->setPosition("path",1); 

  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,argc,argv,"mode path");
  ChangeFileModeFunc apiFunc(path,mode);
  return GenericCli().run(apiFunc, dietConfig, argc, argv);

}
