

#include "connect.hh"
#include "api.h"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

                std::string userId;
                std::string password;
                std::string configFile;
/***************  Default configuration file ***************** */

		{
    
			ifstream f(defaultConfig.c_str());
   
			if (f.is_open()){
				config.setConfigFile(defaultConfig);
			}
   
			f.close();
		}
/**************** Describe options *************/


                UMS_Data::ConnectOptions connectOpt;
		Options opt(&config );
  
		opt.add("version,v", "print version message",GENERIC);
		
                opt.add<string>("dietConfig,c", "The diet config file",ENV);

		opt.add<string>("userId,u","represents the VISHNU user identifier",HIDDEN);

		opt.setPosition("userId",1);

		opt.add<string>("password,w","represents the password od the user",HIDDEN);
		opt.setPosition("password",1);

		opt.add<int>("closePolicy,p","for closing session automatically",CONFIG);

		opt.add<int >("sessionInactivityDelay,d",
				          "The session inactivity delay",CONFIG);

		opt.add<string >("substituteUserId,s","The substitute user identifier",CONFIG);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("help")){

			cout << "Usage: " << av[0] <<" [options] userId"<<endl;
			
			cout << opt << endl;
                   return 0;

		}

		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << opt.get<string>("userId") << endl;
                        userId=opt.get<string>("userId");
		}
		
		if(opt.count("password")){
			
			cout <<"the password is set to: " << opt.get<string >("password") << endl;
                        password=opt.get<string >("password");
		}
		
		if(opt.count("closePolicy")){

			cout << "The close policy is " << opt.get<int>("closePolicy") <<endl;
                        connectOpt.setClosePolicy(opt.get<int>("closePolicy"));
		}


		if (opt.count("sessionInactivityDelay")){
			
			cout <<"The session inactivity delay is " << opt.get< int >("sessionInactivityDelay") << endl;
                        connectOpt.setSessionInactivityDelay(opt.get< int >("sessionInactivityDelay"));
		}
  
		if (opt.count("substituteUserId")){
	
			cout <<"The substitute user identifier is " << opt.get< string >("substituteUserId") << endl;
                        connectOpt.setSubstituteUserId(opt.get< string >("substituteUserId"));
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

                std::string sessionKey;
		int res = connect(userId,password, sessionKey, connectOpt);


		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


