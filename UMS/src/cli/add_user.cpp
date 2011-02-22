#include "addUser.hh"
#include "utils.hh"
#include "userUtils.hpp"
#include<boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){




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
boost::shared_ptr<Options>opt= makeUserOptions(av[0], fUserId,dietConfig,fPrivilege,fFirstname, fLastname,fEmail,1);


        opt->add("sessionKey",
                        "The session key",
                        ENV,
                        sessionKey);


				try{
/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();





/********  Process **************************/

		if (opt->count("userId")){

			cout <<"The user identifier is " << newUser.getUserId() << endl;

		}

		if(opt->count("privilege")){

			cout <<"the privilege is : " << newUser.getPrivilege() << endl;

		}

		if(opt->count("firstname")){

			cout << "The firstname is " << newUser.getFirstname() << endl;

		}


		if (opt->count("lastname")){

			cout <<"The lastname is " << newUser.getLastname() << endl;

		}

		if (opt->count("email")){

			cout <<"The email is " <<newUser.getEmail() << endl;

		}


		checkVishnuConfig(*opt);

		if (opt->count("sessionKey")){

			cout <<"The session key is " << sessionKey << endl;

		}

		else{

			cerr << "Set the VISHNU_SESSION_KEY" <<endl;

			return 1;
		}



/************** Call UMS connect service *******************************/

               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

							 addVishnuUser(sessionKey,newUser);


	}// End of try bloc

catch(po::required_option& e){// a required parameter is missing


  usage(*opt," privilege userId firstname lastname email ","required parameter is missing");
  }

catch(VishnuException& e){// catch all Vishnu runtime error

	      errorUsage(av[0], e.getMsg(),EXECERROR);

				return e.getMsgI() ;
}

  catch(std::exception& e){

    errorUsage(av[0],e.what());

    return 1;
  }

  return 0;

}// end of main

