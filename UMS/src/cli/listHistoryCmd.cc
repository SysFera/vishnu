

#include "listHistoryCmd.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){



		/******* Parsed value containers ****************/

		string dietConfig;

		std::string userId;

		std::string sessionId;

		long startDateOption;

		long endDateOption;

		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListCommands listCmd;

		UMS_Data::ListCmdOptions listOptions;


/**************** Describe options *************/


		Options opt(av[0] );

		opt.add("version,v",
				"print version message",
				GENERIC );

        opt.add("dietConfig,c",
						            "The diet config file",
												ENV,
												dietConfig);

        opt.add("adminListOption,a",
						            "is an admin option for listing all commands of all users	",
												CONFIG);

				opt.add("userId,u",
		                 "an admin option for listing commands launched\n"
										 "by a specific user identified by his/her userId",
										 CONFIG,
										 userId);

				opt.add("sessionId,i",
									    	"lists all commands launched within a specific session",
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


		if (opt.count("adminListOption")){

			cout <<"It is an admin list option " << endl;

			listOptions.setAdminListOption(true);
		}


		if (opt.count("userId")){

			cout <<"The user identifier is " << userId << endl;

			listOptions.setUserId(userId);

		}

		if(opt.count("sessionId")){

			cout <<"the sessionId is : " << sessionId << endl;

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

		if (opt.count("dietConfig")){

			cout <<"The diet config file " << dietConfig << endl;
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


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



							 listHistoryCmd(sessionKey,listCmd,listOptions);


							// Display the list
     if(isEmpty) {
         cout << listCmd << endl;
     }
     else {
         for(int i = 0; i < listCmd.getCommands().size(); i++) {
           cout << listCmd.getCommands().get(i) << endl;
        }
     }

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


