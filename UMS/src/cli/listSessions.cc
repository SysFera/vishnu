

#include "listSessions.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


		/******* Parsed value containers ****************/

		string dietConfig;

		int sessionInactivityDelay;

		std::string machineId;

		std::string userId;

		std::string sessionId;

		long startDateOption;

		long endDateOption;

		std::string sessionKey;

		/********** EMF data ************/


		UMS_Data::StatusType status;

		UMS_Data::SessionCloseType sessionClosePolicy;

		UMS_Data::ListSessions listSession;

		UMS_Data::ListSessionOptions listOptions;

/**************** Describe options *************/


		Options opt(av[0] );

		opt.add("version,v",
				"print version message",
				GENERIC );

        opt.add("dietConfig,c",
						            "The diet config file",
												ENV,
												dietConfig);

		opt.add("status,t",
				        "specifies the type of the sessions which will be\n"
								"listed (INACTIVE or ACTIVE)",
						  CONFIG,
						  status);

		opt.add("sessionClosePolicy,p",
				      "Specifies the closure mode of the sessions which\n"
							"will be listed (CLOSE_ON_TIMEOUT or CLOSE_ON_DISCONNECT)",
						  CONFIG,
						  sessionClosePolicy);

		opt.add("sessionInactivityDelay,d",
				      "specifies the inactivity delay in seconds\n"
							"of the sessions which will be listed",
					  CONFIG,
					  sessionInactivityDelay);

		opt.add("machineId,m",
				       "allows the user to list sessions\n"
							 "opened on a specific machine",
						CONFIG,
						machineId);

		opt.add("adminListOption,a",
				    "is an admin option for listing\n"
						"all sessions of all users",
						CONFIG);

		opt.add("userId,u",
				        "is an admin option for listing\n"
								"sessions opened by a specific user	",
						    CONFIG,
						    userId);

		opt.add("sessionId,i",
				        "allows the user to list all commands\n"
								"launched within a specific session",
						    CONFIG,
						    sessionId);

				opt.add("startDateOption,s",
									    	"allows the user to organize the commands listed\n"
												"by providing the start date\n"
												"(the UNIX timestamp	of the start date is used)",
												CONFIG,
												startDateOption);

				opt.add("endDateOption,e",
									    	"allows the user to organize the commands listed\n"
												"by providing the end date (the timestamp of the end date is used).\n"
												"By default, the end date is the current day",
												CONFIG,
												endDateOption);


				opt.add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

	try {
/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		bool isEmpty=opt.empty();//if no value was given in the command line

		opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();


/********  Process **************************/


		if (opt.count("status")){

			cout <<"The session state type is " << status <<endl;

			listOptions.setStatus(status);
		}

		if (opt.count("sessionClosePolicy")){

			cout <<"The session close policy is " << sessionClosePolicy<< endl;

			listOptions.setSessionClosePolicy(sessionClosePolicy);
		}

		if (opt.count("sessionInactivityDelay")){

			cout <<"The session inactivity delay is " << sessionInactivityDelay<< endl;

			listOptions.setSessionInactivityDelay(sessionInactivityDelay);
		}


		if (opt.count("machineId")){

			cout <<"The machine identifier is " << machineId << endl;

			listOptions.setMachineId(machineId);
		}

		if (opt.count("adminListOption")){

			cout <<"It is an admin list option " << endl;

			listOptions.setAdminListOption(true);
		}


		if (opt.count("userId")){

			cout <<"The user identifier is " << userId << endl;

			listOptions.setUserId(userId);

		}

		if (opt.count("sessionId")){

			cout <<"The session identifier is " << sessionId << endl;

			listOptions.setSessionId(sessionId);

		}

		if(opt.count("startDateOption")){

			cout <<"the start date option is : " << startDateOption << endl;

			listOptions.setStartDateOption(startDateOption );
		}


		if(opt.count("endDateOption")){

			cout <<"the end date option is : " << endDateOption << endl;

			listOptions.setEndDateOption(endDateOption );
		}


		if(opt.count("sessionKey")){

			cout <<"the session key is : " << sessionKey << endl;
		}


		checkVishnuConfig(opt);

		if ( opt.count("help")){

			cout << "Usage: " << av[0] <<" [options]  "<<endl;

			cout << opt << endl;

			return 0;
		}




/************** Call UMS connect service *******************************/


               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }



							int res = listSessions(sessionKey,listSession,listOptions);


							// Display the list
      if(isEmpty) {
          cout << listSession << std::endl;
      }
      else {
            for(int i = 0; i < listSession.getSessions().size(); i++) {
              cout << listSession.getSessions().get(i) << endl;
            }
       }

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


