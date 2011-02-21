#include "deleteUser.hh"
#include "utils.hh"
#include "connectUtils.hpp"
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

		opt->add("sessionKey,s","The session Key",ENV,sessionKey);


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

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

							 deleteUser(sessionKey, userId);


	}// End of try bloc

catch(po::required_option& e){// a required parameter is missing

  usage(*opt," userId","required parameter is missing");
  }
  catch(std::exception& e){

    errorUsage(av[0],e.what());

    return 1;
  }

  return 0;

}// end of main


