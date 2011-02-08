#include "reconnect.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	try {
                string userId;
                string password;
                string sessionId;
                string sessionKey;
                string dietConfig;

		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

		int reqParam=0;   // to count the required parameters for the command

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

        opt.add("dietConfig,c",
				        "The diet config file",
						ENV,
						dietConfig);

        opt.add("sessionKey,s",
				        "The sessionKey",
						ENV,
						sessionKey);

		opt.add("userId,u",
						"represents the VISHNU user identifier",
						HIDDEN,
						userId);

		opt.setPosition("userId",1);


		 opt.add("sessionId,s",
				 		 "represents the identifier of the session",
						 HIDDEN,
						 sessionId);

		 opt.setPosition("sessionId",1);




/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		//opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/
		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;

			reqParam=reqParam+1;
		}
		
		
		if(opt.count("sessionId")){

			cout << "The session identifier is " << sessionId <<endl;

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


		 if ((reqParam < RCPARAM)|| (opt.count("help"))){

			 cout << "Usage: " << av[0] <<" [options] userId sessionId"<<endl;

			 cout << opt << endl;

			 return 0;

		 }
		     
		 else{

					 password=getpass("Password: ");

				cout << "The user passwor is " << password << endl;	
		 }


/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
                   
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


