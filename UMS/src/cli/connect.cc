
#include "connect.hh"
#include "utils.hh"
#include "connectUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;


int main (int ac, char* av[]){


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


/**************** Describe options *************/

			boost::shared_ptr<Options> opt=makeConnectOptions(av[0],userId,1,dietConfig);

			opt->setPosition("userId",-1);

			opt->add("sessionInactivityDelay,d",
				      "The session inactivity delay",
					  CONFIG,
					  fSessionInactivityDelay);


			opt->add("substituteUserId,s",
				         "The substitute user identifier",
						 CONFIG,
						 fSubstituteUserId);

		opt->add("closePolicy,p",
           "for closing session automatically",
           CONFIG,
             fClosePolicy );

		try{


/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();


/********  Process **************************/


		checkVishnuConfig(*opt);

		//Fix me

			password= takePassword("Password: ",userId);// uses getpass which is obsolete.



/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }


							 connect(userId,password, sessionKey, connectOpt);


			// store sessionKey into ./session

							/* sessionKey="myfirstSession";
							 std::string sessionFile=getSessionLocation(getppid());
							 cout << "sessionFile: " << sessionFile<< endl;
							 SessionEntry session(sessionKey,connectOpt.getClosePolicy());
							 storeLastSession(session,sessionFile.c_str());*/

	}// End of try bloc

	catch(po::required_option& e){// a required parameter is missing

			     usage(*opt,"[options] userId ","required parameter is missing");
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


