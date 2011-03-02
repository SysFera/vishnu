/**
 * \file change_password.cpp
 * This file defines the VISHNU change password command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "common.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include "utilVishnu.hpp"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


  string userId;

  string oldPassword;

  string newPassword;

  string sessionKey;

  string dietConfig;

  /**************** Describe options *************/

  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt.add("userId",
          "The VISHNU user identifier",
          HIDDEN,
          userId,
          1);
  opt.setPosition("userId",-1);


  try {


    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    opt.parse_env(env_name_mapper());

    opt.notify();



    /********  Process **************************/


    if (opt.count("help")){

      helpUsage(opt,"[options]");

      return 0;

    }


    checkVishnuConfig(opt);



    oldPassword=vishnu::takePassword("old password: ");

    cout << "old password is " << oldPassword <<endl;

    newPassword=vishnu::takePassword("new password: ");

    cout << "The new password is "<< newPassword <<endl;




    /************** Call UMS connect service *******************************/


    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      cerr << "DIET initialization failed !" << endl;

      return 1;
    }



    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      changePassword(userId,oldPassword, newPassword);


    }




  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(opt,"[options] userId ","required parameter is missing");
  }

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


