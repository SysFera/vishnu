

#include "addVishnuUser.hh"

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
        
		std::string firstname;

		std::string lastname;

		int privilege; 
		
		std::string email; 
		
		std::string sessionKey; 

		/********** EMF data ************/

		UMS_Data::User newUser;




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

				opt.add("sessionKey,s",
	                        "The session key",
													ENV,
												  sessionKey);
				
				opt.add("privilege",
		                 "the privilege of the user (admin or simple user)",
										 HIDDEN,
										 privilege);

				opt.setPosition("privilege",1);


				opt.add("userId,u",
									    	"represents the VISHNU user identifier",
												HIDDEN,
												userId);

				opt.setPosition("userId",1);

				opt.add("firstname",
												"The firstname of the user",
												HIDDEN,
												firstname);

				opt.setPosition("firstname",1);


				opt.add("lastname",
												"The lastname of the user",
												HIDDEN,
												lastname);

				opt.setPosition("lastname",1);


				opt.add("email",
												"The email of the user",
												HIDDEN,
												email);
										
				opt.setPosition("email",1);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/
		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
			
			newUser.setUserId(userId);
			
			reqParam=reqParam+1;
		}
		
		if(opt.count("privilege")){
			
			cout <<"the privilege is : " << privilege << endl;
			
			newUser.setPrivilege(privilege);
			
			reqParam=reqParam+1;
		}
		
		if(opt.count("firstname")){

			cout << "The firstname is " << firstname << endl;
			
			newUser.setFirstname(firstname);
			
			reqParam=reqParam+1;
                        
		}


		if (opt.count("lastname")){
			
			cout <<"The lastname is " << lastname << endl;
			
			newUser.setLastname(lastname);
			
			reqParam=reqParam+1;

		}
  
		if (opt.count("email")){
	
			cout <<"The email is " << email << endl;
		
			newUser.setEmail(email);
	
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
/*
               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = addVishnuUser(sessionKey,newUser);


	*/	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


