/**
 * \file chgrp.cpp
 * This file defines the VISHNU chgrp command
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

struct ChangeFileGroupFunc {

  std::string mpath;
  std::string mgroup;

  ChangeFileGroupFunc(const std::string& path,const std::string& group):mpath(path),mgroup(group){}

  int operator()(std::string sessionKey) {

      int res =chgrp(sessionKey, mgroup, mpath);
    return res;
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  std::string configFile;
  std::string sessionKey;
  std::string path;
  std::string group;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=processOpt(argv[0], configFile);

  opt->add("group,g",
      "The new group owner of file/directory",
      HIDDEN,
      group,1);
  opt->setPosition("group",1);

  opt->add("path,p",
      "The file/directory following the pattern [host:]file path",
      HIDDEN,
      path,1);
  opt->setPosition("path",1);

  opt->add("sessionkey,k",
      "VISHNU session key to connect",
      ENV,
      sessionKey);

  bool isEmpty;
  GenericCli().processListOpt( opt, isEmpty,argc,argv);
  ChangeFileGroupFunc apiFunc(path,group);
  return GenericCli().run(apiFunc, configFile, argc, argv, sessionKey);

}
