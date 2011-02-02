#include "reconnect.hh"
#include "api.h"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	try {
                string userId;
                string password;
                string sessionId;
                string sessionKey;
                string configFile;

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
  
		opt.add("version,v", "print version message",GENERIC);

                opt.add<string>("dietConfig,c", "The diet config file",ENV);

		opt.add<string>("userId,u","represents the VISHNU user identifier",HIDDEN);

		opt.setPosition("userId",1);

		opt.add<string>("password,w","represents the password od the user",HIDDEN);
		opt.setPosition("password",2);

		 opt.add<string>("sessionId,s","represents the identifier of the session",HIDDEN);
		 opt.setPosition("sessionId",3);




/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		//opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("help")){

			 cout << "Usage: " << av[0] <<" [options] userId sessionId"<<endl;

			cout << opt << endl;

		}

		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << opt.get<string>("userId") << endl;
                        userId = opt.get<string>("userId");
		}
		
		if(opt.count("password")){
			
			cout <<"the password is set to: " << opt.get<string >("password") << endl;
                        password = opt.get<string >("password");
		}
		
		if(opt.count("sessionId")){

			cout << "The session identifier is " << opt.get<string>("sessionId") <<endl;
                        sessionId = opt.get<string>("sessionId");
		}

	        if (opt.count("dietConfig")){

                        cout <<"The diet config file " << opt.get< string >("dietConfig") << endl;
                        configFile = opt.get< string >("dietConfig");

                }

/************** Call UMS connect service *******************************/

               // initializing DIET
              if (diet_initialize(configFile.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }
	
              reconnect(userId, password, sessionId, sessionKey);

		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


