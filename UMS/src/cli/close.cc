#include "close.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string sessionKey;


	string dietConfig;

/**************** Describe options *************/


		Options opt(av[0] );


        opt.add("dietConfig,c",
						"The diet config file",
						ENV,
						dietConfig);

		opt.add("sessionKey,s",
						 "The session Key",
						 ENV,
						 sessionKey);

	try {

/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		//opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("help")){

			helpUsage(opt,"[options]");

			return 0;

		}


		if (opt.count("sessionKey")){

			cout <<"your session key is " << sessionKey << endl;
		}


		if (opt.count("dietConfig")){


			cout <<"The diet config file " << dietConfig << endl;
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


/************** Call UMS connect service *******************************/


		if (diet_initialize(dietConfig.c_str(), ac, av)) {

			cerr << "DIET initialization failed !" << endl;

			return 1;

		}

		int res=close(sessionKey);




	}// End of try bloc

	catch(std::exception& e){
		errorUsage(av[0],e.what());
		return 1;
	}

	return 0;

}// end of main


