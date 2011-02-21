#include "changePassword.hh"
#include "utils.hh"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){



	string oldPassword;

	string newPassword;

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

		opt.parse_env(env_name_mapper());

		opt.notify();



/********  Process **************************/


		if (opt.count("help")){

      helpUsage(opt,"[options]");

      return 0;

    }


		checkVishnuConfig(opt);

		if (opt.count("sessionKey")){

			cout <<"The sessionKey is " << sessionKey << endl;
		}



			oldPassword=getpass("old password: ");

			cout << "old password is" << oldPassword <<endl;

			newPassword=getpass("new password: ");

			cout << "The new password is "<< newPassword <<endl;





/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              changePassword(sessionKey, oldPassword, newPassword);



	}// End of try bloc

  catch(std::exception& e){

    errorUsage(av[0], e.what());
    return 1;
  }

  return 0;

}//

