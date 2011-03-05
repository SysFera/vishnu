/**
 * \file save_configuration.cpp
 * This file defines the VISHNU save configuration command 
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

  /********** EMF Data ****************************/


  UMS_Data::Configuration configuration;


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


    checkVishnuConfig(opt);

    if(opt.count("help")){

      helpUsage(opt,"[options]");

      return 0;
    }


    /************** Call UMS save configuration service *******************************/


    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

      cerr << "DIET initialization failed !" << endl;

      return 1;
    }


    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<"the current sessionkey is: " << sessionKey <<endl;

      saveConfiguration(sessionKey,configuration);

      std::cout<< "The configuration path is : " <<configuration.getFilePath() <<std::endl; 

      printSuccessMessage();
    }



  }// End of try bloc

  catch(VishnuException& e){// catch all Vishnu runtime error

    errorUsage(av[0], e.getMsg(),EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){
    errorUsage(av[0],e.what());
    return 1;
  }

  return 0;

}// end of main


