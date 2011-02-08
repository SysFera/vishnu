

#include "connect.hh"

namespace po = boost::program_options;

using namespace std;


int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";
		
		int reqParam=0;   // to count the required parameters for the command

		/******* Parsed value containers ****************/
       
		string dietConfig;

		std::string userId;
        
		std::string password;

		int closePolicy;

		int sessionInactivityDelay;

		std::string substituteUserId;

		/********** EMF Data ****************************/


  		UMS_Data::ConnectOptions connectOpt;


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
				GENERIC );
		
        opt.add("dietConfig,c", 
				        "The diet config file",
						ENV,
						dietConfig);

		opt.add("userId,u",
				        "represents the VISHNU user identifier",
						HIDDEN,
						userId);

		opt.setPosition("userId",-1);

		opt.add("closePolicy,p",
					 "for closing session automatically",
					 CONFIG,
					 closePolicy);

		opt.add("sessionInactivityDelay,d",
				      "The session inactivity delay",
					  CONFIG,
					  sessionInactivityDelay);

		opt.add("substituteUserId,s",
				         "The substitute user identifier",
						 CONFIG,
						 substituteUserId);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
			
			reqParam=reqParam+1;

		}
	 
   		
		if(opt.count("closePolicy")){

			cout << "The close policy is " << closePolicy <<endl;
                        
			connectOpt.setClosePolicy(closePolicy);
		}


		if (opt.count("sessionInactivityDelay")){
			
			cout <<"The session inactivity delay is " << sessionInactivityDelay << endl;

			connectOpt.setSessionInactivityDelay(sessionInactivityDelay);
		}
  
		if (opt.count("substituteUserId")){
	
			cout <<"The substitute user identifier is " << substituteUserId << endl;
			
			connectOpt.setSubstituteUserId(substituteUserId);
		}

            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;
                  
		}
		else{
			
			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
			
		}


		
		if ( (reqParam<CPARAM) || (opt.count("help"))) {


			      cerr << "Usage: " << av[0] <<" [options] userId"<<endl;
						    
						cerr << opt << endl;
						
						return 1;
						
		}
		  
		else{

			password= getpass("Password: ");// getpass is obsolete. 

			cout << "The user password  is " << password << endl;

							 
		}




/************** Call UMS connect service *******************************/

		
               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
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


