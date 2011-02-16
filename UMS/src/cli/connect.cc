

#include "connect.hh"
#include "utils.hh"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;


int main (int ac, char* av[]){


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";


		/******* Parsed value containers ****************/

		string dietConfig;

		std::string userId;

		std::string password;


		/********** EMF Data ****************************/


  		UMS_Data::ConnectOptions connectOpt;


			/******** Callback functions ******************/

			boost::function1<void,UMS_Data::SessionCloseType> fClosePolicy( boost::bind(&UMS_Data::ConnectOptions::setClosePolicy,boost::ref(connectOpt),_1));
			boost::function1<void,int> fSessionInactivityDelay( boost::bind(&UMS_Data::ConnectOptions::setSessionInactivityDelay,boost::ref(connectOpt),_1));
			boost::function1<void,string> fSubstituteUserId( boost::bind(&UMS_Data::ConnectOptions::setSubstituteUserId,boost::ref(connectOpt),_1));

			/*********** Out parameters *********************/

			std::string sessionKey;


/***************  Default configuration file ***************** */

		{

			ifstream f(defaultConfig.c_str());

			if (f.is_open()){
				config.setConfigFile(defaultConfig);
			}

			f.close();
		}
/**************** Describe options *************/

		Options opt(&config );

		opt.add("version,v",
				"print version message",
				GENERIC );

        opt.add("dietConfig,c",
				        "The diet config file",
						ENV,
						dietConfig);

		opt.add("userId,u",
				        "represents the VISHNU user identifier",
						HIDDEN,
						userId,
						1);

		opt.setPosition("userId",-1);

		opt.add("closePolicy,p",
					 "for closing session automatically",
					 CONFIG,
			       fClosePolicy );


		opt.add("sessionInactivityDelay,d",
				      "The session inactivity delay",
					  CONFIG,
					  fSessionInactivityDelay);

		opt.add("substituteUserId,s",
				         "The substitute user identifier",
						 CONFIG,
						 fSubstituteUserId);


		try{


/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();


/********  Process **************************/

		if (opt.count("dietConfig")==0){

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;

		}

			//Fix me

			password= takePassword("Password: ",userId);// uses getpass which is obsolete.



/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }


							int res = connect(userId,password, sessionKey, connectOpt);

	}// End of try bloc

	catch(po::required_option& e){// a required parameter is missing

		if ( opt.count("help")) { // unless help is needed

			      usage(opt,"[options] userId ");

		}
		else{

		cerr << e.what() <<endl;

		cerr << "To get help, try <<"<< av[0] << " -h >>"<< endl;

		}
		return 1;
	}
	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


