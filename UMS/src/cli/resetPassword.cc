#include "resetPassword.hh"
#include "connectUtils.hpp"
#include "utils.hh"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string userId;

	string sessionKey;

	string dietConfig;


/**************** Describe options *************/
	boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

	      opt->setPosition("userId",-1);


				opt->add("sessionKey,s",
						           "The session key",
											 ENV,
											 sessionKey);



	try {

/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();

/********  Process **************************/

		if (opt->count("userId")){

			cout <<"The user identifier is " << userId << endl;

		}


		if (opt->count("dietConfig")){


			cout <<"The diet config file " << dietConfig << endl;

		}

		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


		if(opt->count("sessionKey")){

			cout << "The session Key is: " << sessionKey <<endl;

		}


/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              resetPassword(sessionKey,userId);



	}// End of try bloc



	catch(po::required_option& e){// a required parameter is missing


		usage(*opt,"[options] userId ","required parameter is missing");

	}
  catch(std::exception& e){


		errorUsage(av[0], e.what());

		return 1;

	}

	  return 0;

}// end of main


