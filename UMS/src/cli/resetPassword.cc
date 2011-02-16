#include "resetPassword.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string userId;

	string sessionKey;

	string dietConfig;

		int reqParam=0;   // to count the required parameters for the command


/**************** Describe options *************/


		Options opt(av[0] );

		opt.add("version,v",
				"print version message",
				GENERIC);

        opt.add("dietConfig,c",
				        "The diet config file",
						ENV,
						dietConfig);

				opt.add("sessionKey,s",
						           "The session key",
											 ENV,
											 sessionKey);

		opt.add("userId,u",
						"represents the VISHNU user identifier",
						HIDDEN,
						userId);

		opt.setPosition("userId",-1);



	try {

/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		//opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("userId")){

			cout <<"The user identifier is " << userId << endl;

			reqParam=reqParam+1;
		}


		if (opt.count("dietConfig")){


			cout <<"The diet config file " << dietConfig << endl;

		}

		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


		if(opt.count("sessionKey")){

			cout << "The session Key is: " << sessionKey <<endl;

		}


		 if ((reqParam < RPPARAM)|| (opt.count("help"))){

			 cout << "Usage: " << av[0] <<" userId"<<endl;

			 cout << opt << endl;

			 return 0;
		 }




/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              resetPassword(sessionKey,userId);



	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


