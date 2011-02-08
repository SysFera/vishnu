#include "changePassword.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	try {
                string userId;
                string oldPassword;
                string newPassword;
                string sessionId;
                string sessionKey;
                string dietConfig;

		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

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
				GENERIC);

        opt.add<string>("dietConfig,c",
				        "The diet config file",
						ENV,
						dietConfig);

		opt.add<string>("userId,u",
						"represents the VISHNU user identifier",
						HIDDEN,
						userId);

		opt.setPosition("userId",1);






/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		//opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if ((ac < 2)|| (opt.count("help"))){

			 cout << "Usage: " << av[0] <<" userId"<<endl;

			cout << opt << endl;

			return 0;

		}
		else{

			oldPassword=getpass("old password: ");
			
			newPassword=getpass("new password: ");

		}

		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
		}
		
		if(!oldPassword.empty()){
			
			cout <<"the old password was: " << oldPassword << endl;
		}
		
		if(!newPassword.empty()){

			cout << "The new password is: " << newPassword << endl;

		}



		if (opt.count("dietConfig")){

                      
			cout <<"The diet config file " << dietConfig << endl;

                }

/************** Call UMS connect service *******************************/

/*
               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
                   
				  cerr << "DIET initialization failed !" << endl;
               
				  return 1;
              }
	
              changePassword(userId, oldPassword, newPassword);
*/
	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


