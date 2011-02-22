#include "changePassword.hh"
#include "utils.hh"
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){




	string userId;

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

				opt.add("userId",
				        "The VISHNU user identifier",
						HIDDEN,
						userId,
						1);
				opt.setPosition("userId",-1);

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



			oldPassword=takePassword("old password: ",userId);

			cout << "old password is" << oldPassword <<endl;

			newPassword=takePassword("new password: ",userId);

			cout << "The new password is "<< newPassword <<endl;





/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              changePassword(userId,oldPassword, newPassword);



	}// End of try bloc

 catch(po::required_option& e){// a required parameter is missing

           usage(opt,"[options] userId ","required parameter is missing");
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


