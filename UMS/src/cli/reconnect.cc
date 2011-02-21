#include "reconnect.hh"
#include "utils.hh"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string userId;

	string password;

	string sessionId;

	string sessionKey;

	string dietConfig;



/**************** Describe options *************/


			boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1, dietConfig);

			opt->setPosition("userId",1);


        opt->add("sessionKey,s",
				        "The sessionKey",
						ENV,
						sessionKey);

		 opt->add("sessionId,s",
				 		 "represents the identifier of the session",
						 HIDDEN,
						 sessionId,1);

		 opt->setPosition("sessionId",1);



	try {

/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();





/********  Process **************************/

		if (opt->count("userId")){

			cout <<"The user identifier is " << userId << endl;

		}


		if(opt->count("sessionId")){

			cout << "The session identifier is " << sessionId <<endl;

		}

		if (opt->count("dietConfig")){

			cout <<"The diet config file " << dietConfig << endl;
		}

		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


		if (opt->count("sessionKey")){

			cout <<"The session key is " << sessionKey << endl;
		}



		//Fix me
					 password=takePassword("Password: ",userId);

				cout << "The user password is " << password << endl;



/************** Call UMS connect service *******************************/

/*
               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }
*/

				   // get the sessionKey

							 std::string sessionFile=getSessionLocation(getppid());

							 SessionEntry session=getLastSession(sessionFile);

							 sessionKey=session.getSessionKey();

							 cout <<"sessionKey " << sessionKey <<endl;

              //reconnect(userId, password, sessionId, sessionKey);



	}// End of try bloc

	catch(po::required_option& e){

			usage(*opt,"[options] userId sessionId","required parameter is missing");

	}

	catch(std::exception& e){
		errorUsage(av[0], e.what()) ;
		return 1;
	}

	return 0;

}// end of main


