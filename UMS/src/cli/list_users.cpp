/**
 * \file list_users.cpp
 * This file defines the VISHNU list user command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string userIdOption;

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListUsers lsUsers;

  /**************** Describe options *************/

  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt.add("userIdOption,i",
          "An option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG,
          userIdOption);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    bool isEmpty=opt.empty();//if no value was given in the command line

    opt.parse_env(env_name_mapper());

    opt.notify();


    /********  Process **************************/

    checkVishnuConfig(opt);

    if ( opt.count("help")){

      helpUsage(opt," [options]");

      return 0;
    }

    /************** Call UMS list users service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      listUsers(sessionKey,lsUsers, userIdOption);

    }


    // Display the list
    if(isEmpty) {
      cout << lsUsers << endl;
    }
    else {
      for(int i = 0; i < lsUsers.getUsers().size(); i++) {
        cout << lsUsers.getUsers().get(i) << endl;
      }
    }
    printSuccessMessage();
  }// End of try bloc

  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){

    errorUsage(av[0], e.what());

    return 1;
  }

  return 0;

}// end of main


