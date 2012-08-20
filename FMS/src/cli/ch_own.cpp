/**
 * \file ch_grp.cpp
 * This file defines the VISHNU ch_grp command
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "sessionUtils.hpp"
#include "cmdArgs.hpp"
#include <boost/bind.hpp>
#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ChangeFileOwnerFunc {

  std::string mpath;
  std::string mown;

  ChangeFileOwnerFunc(const std::string& path,const std::string& user):mpath(path),mown(user){}

  int operator()(std::string sessionKey) {

      int res =chOwn(sessionKey, mown, mpath);
    return res;
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string path;
  string user;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=processOpt(argv[0], dietConfig);

  opt->add("owner,o",
      "The new group owner of file/directory following the pattern [user][:[group]]",
      HIDDEN,
      user,1);
  opt->setPosition("owner",1);

  opt->add("path,p",
      "The file/directory following the pattern [host:]file path",
      HIDDEN,
      path,1);
  opt->setPosition("path",1);

  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,argc,argv,"owner path");
  ChangeFileOwnerFunc apiFunc(path,user);
  return GenericCli().run(apiFunc, dietConfig, argc, argv);

}
