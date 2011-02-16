#include "addUser.hh"
#include<boost/bind.hpp>
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){



	try {



		int reqParam=0;   // to count the required parameters for the command

		string sessionKey;

		/******* Parsed value containers ****************/

		string dietConfig;

		/********** EMF data ************/

		UMS_Data::User newUser;

			/******** Callback functions ******************/

			boost::function1<void,string> fUserId( boost::bind(&UMS_Data::User::setUserId,boost::ref(newUser),_1));
			boost::function1<void,UMS_Data::PrivilegeType> fPrivilege( boost::bind(&UMS_Data::User::setPrivilege,boost::ref(newUser),_1));
			boost::function1<void,string> fFirstname( boost::bind(&UMS_Data::User::setFirstname,boost::ref(newUser),_1));
			boost::function1<void,string> fLastname( boost::bind(&UMS_Data::User::setLastname,boost::ref(newUser),_1));
			boost::function1<void,string> fEmail( boost::bind(&UMS_Data::User::setEmail,boost::ref(newUser),_1));


/**************** Describe options *************/



		Options opt(av[0] );

		opt.add("version,v",
				"print version message",
				GENERIC );

        opt.add("dietConfig,c",
						            "The diet config file",
												ENV,
												dietConfig);

				opt.add("sessionKey,s",
	                        "The session key",
													ENV,
												  sessionKey);

				opt.add("privilege",
		                 "the privilege of the user (admin or simple user)",
										 HIDDEN,
										 fPrivilege);

				opt.setPosition("privilege",1);


				opt.add("userId,u",
									    	"represents the VISHNU user identifier",
												HIDDEN,
												fUserId);

				opt.setPosition("userId",1);

				opt.add("firstname",
												"The firstname of the user",
												HIDDEN,
												fFirstname);

				opt.setPosition("firstname",1);


				opt.add("lastname",
												"The lastname of the user",
												HIDDEN,
												fLastname);

				opt.setPosition("lastname",1);


				opt.add("email",
												"The email of the user",
												HIDDEN,
												fEmail);

				opt.setPosition("email",1);

/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		opt.parse_cfile();

		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("userId")){

			cout <<"The user identifier is " << newUser.getUserId() << endl;

			reqParam=reqParam+1;
		}

		if(opt.count("privilege")){

			cout <<"the privilege is : " << newUser.getPrivilege() << endl;

			reqParam=reqParam+1;
		}

		if(opt.count("firstname")){

			cout << "The firstname is " << newUser.getFirstname() << endl;

			reqParam=reqParam+1;
		}


		if (opt.count("lastname")){

			cout <<"The lastname is " << newUser.getLastname() << endl;

			reqParam=reqParam+1;
		}

		if (opt.count("email")){

			cout <<"The email is " <<newUser.getEmail() << endl;

			reqParam=reqParam+1;
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

		else{

			cerr << "Set the VISHNU_SESSION_KEY" <<endl;

			return 1;
		}


		if ((reqParam < AVUPARAM) || (opt.count("help"))){

			cout << "Usage: " << av[0] <<" privilege userId firstname lastname email"<<endl;

			cout << opt << endl;

			return 1;

		}



/************** Call UMS connect service *******************************/

               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }



							int res = addVishnuUser(sessionKey,newUser);




	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


