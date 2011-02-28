#include "common.hpp"
#include "utils.hpp"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include<boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){



  /******* Parsed value containers ****************/

  string dietConfig;

  string sessionKey;

  std::string userId;



  /**************** Describe options *************/

  boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

  opt->setPosition("userId",-1);


  try {

    /**************  Parse to retrieve option values  ********************/

    opt->parse_cli(ac,av);

    opt->parse_env(env_name_mapper());

    opt->notify();


    /********  Process **************************/

    if (opt->count("userId")){

      cout <<"The user identifier is " << userId << endl;

    }



    checkVishnuConfig(*opt);

    /************** Call UMS connect service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
      cerr << "DIET initialization failed !" << endl;
      return 1;
    }

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout << "the current session key is " << sessionKey <<endl;

      deleteUser(sessionKey, userId);

    }



  }// End of try bloc

  catch(po::required_option& e){// a required parameter is missing

    usage(*opt," userId","required parameter is missing");
  }

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


