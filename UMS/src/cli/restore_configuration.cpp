#include "restore_configuration.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


  string filePath;

  string sessionKey;

  string dietConfig;

  /**************** Describe options *************/

  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);


  opt.add("filePath",
          "The path of the VISHNU configuration file",
          HIDDEN,
          filePath,
          1);

  opt.setPosition("filePath",-1);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    opt.parse_env(env_name_mapper());

    opt.notify();


    /********  Process **************************/

    checkVishnuConfig(opt);

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

      restoreConfiguration(sessionKey, filePath);

    }


  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(opt,"[options] filePath ","required parameter is missing");
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


