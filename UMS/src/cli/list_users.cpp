/**
 * \file list_users.cpp
 * This file defines the VISHNU list user command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListUserFunc {

  UMS_Data::ListUsers mlsUsers;
  std::string muserIdOption;
  bool mfull;

  ListUserFunc(UMS_Data::ListUsers lsUsers, std::string userIdOption, bool full):
    mlsUsers(lsUsers), muserIdOption(userIdOption), mfull(full)
  {};

  int operator()(std::string sessionKey) {

    int res = listUsers(sessionKey, mlsUsers, muserIdOption);
    // Display the list
    if(mfull) {
      cout << mlsUsers << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsUsers.getUsers().size(); i++) {
        cout << mlsUsers.getUsers().get(i) << endl;
      }
    }

    return res;
  }
};

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string userIdOption;

  /********** EMF data ************/

  UMS_Data::ListUsers lsUsers;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(av[0]));

  opt->add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt->add("userIdOption,i",
          "An option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG,
          userIdOption);

   CLICmd cmd = CLICmd (ac, av, opt);

  try {
    opt->parse_cli(ac,av);
  } catch(po::error& e){ // catch all other bad parameter errors
    errorUsage(av[0], e.what());
    return CLI_ERROR_INVALID_PARAMETER;
  }

  bool isEmpty=opt->empty();//if no value was given in the command line

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[option]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[option]");
    return 0;
  }

  /********  Process **************************/

  bool full = false;
  // Display the list
  if(isEmpty) {
    full = true;
  }

  ListUserFunc listFunc(lsUsers, userIdOption, full);
  return GenericCli().run(listFunc, dietConfig, ac, av);

}// end of main


