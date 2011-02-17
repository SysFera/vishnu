#include "reconnect.hh"
#include "utils.hh"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string userId;

	string password;

	string sessionId;

	string sessionKey;

	string dietConfig;



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
				        "The sessionKey",
						ENV,
						sessionKey);

		opt.add("userId,u",
						"represents the VISHNU user identifier",
						HIDDEN,
						userId,1);

		opt.setPosition("userId",1);


		 opt.add("sessionId,s",
				 		 "represents the identifier of the session",
						 HIDDEN,
						 sessionId,1);

		 opt.setPosition("sessionId",1);



	try {

/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		//opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("userId")){

			cout <<"The user identifier is " << userId << endl;

		}


		if(opt.count("sessionId")){

			cout << "The session identifier is " << sessionId <<endl;

		}

		if (opt.count("dietConfig")){

			cout <<"The diet config file " << dietConfig << endl;
		}

		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


		if (opt.count("sessionKey")){

			cout <<"The session key is " << sessionKey << endl;
		}



		//Fix me
					 password=takePassword("Password: ",userId);

				cout << "The user password is " << password << endl;



/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              reconnect(userId, password, sessionId, sessionKey);



	}// End of try bloc

	catch(po::required_option& e){

		 if ( opt.count("help")){

			helpUsage(opt,"[options] userId sessionId");

			 return 0;
		 }

		 else{

			errorUsage(opt,e.what());


		return 1;

		}


	}

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


