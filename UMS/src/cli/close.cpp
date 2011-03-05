/**
 * \file close.cpp
 * This file defines the VISHNU close command 
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "common.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  string sessionKey;


  string dietConfig;

  /**************** Describe options *************/


  Options opt(av[0] );


  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

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

    /************** Call UMS close service *******************************/

    
       if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

       cerr << "DIET initialization failed !" << endl;

       return 1;

       }
       

    // get the sessionKey



    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      std::string sessionFile=getSessionLocation(getppid());

      cout <<"the session with sessionkey: " << sessionKey << "  is closed"<<endl;

      close(sessionKey);

      removeLastSession(sessionFile);
    }



  }// End of try bloc

  catch(VishnuException& e){// catch all Vishnu runtime error

    if(ERRCODE_SESSIONKEY_EXPIRED==e.getMsgI()){
    
      std::string sessionFile=getSessionLocation(getppid());
      removeLastSession(sessionFile);
    }

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){// catch all std runtime error
    errorUsage(av[0],e.what());
    return 1;
  }

  catch(...){// catch other runtime error
    errorUsage(av[0],"unknown error",EXECERROR);
    return 1;
  }



  return 0;

}// end of main


