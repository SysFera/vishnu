

#include "listUsers.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


		/******* Parsed value containers ****************/

		string dietConfig;

		std::string userIdOption;

		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListUsers lsUsers;

/**************** Describe options *************/

		Options opt(av[0] );

        opt.add("dietConfig,c",
						            "The diet config file",
												ENV,
												dietConfig);

				opt.add("userIdOption,i",
										 "An option for listing all default option values\n"
										 "defined by VISHNU administrator",
										 CONFIG,
										 userIdOption);

				opt.add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

	try {

/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		bool isEmpty=opt.empty();//if no value was given in the command line

		opt.parse_env(env_name_mapper());

		opt.notify();


/********  Process **************************/

		checkVishnuConfig(opt);

		if ( opt.count("help")){

			helpUsage(opt," [options]");

			return 0;
		}

/************** Call UMS connect service *******************************/

               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

							 listUsers(sessionKey,lsUsers, userIdOption);


							// Display the list
      if(isEmpty) {
         cout << lsUsers << endl;
      }
      else {
         for(int i = 0; i < lsUsers.getUsers().size(); i++) {
           cout << lsUsers.getUsers().get(i) << endl;
         }
      }

	}// End of try bloc

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


