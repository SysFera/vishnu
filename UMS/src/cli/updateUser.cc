

#include "updateUser.hh"

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
				
				opt.add("privilege,p",
		                 "the privilege of the user (admin or simple user)",
										 CONFIG,
										 privilege);


				opt.add("userId,u",
									    	"represents the VISHNU user identifier",
												HIDDEN,
												userId);

				opt.setPosition("userId",-1);

				opt.add("firstname,f",
												"The firstname of the user",
												CONFIG,
												firstname);

				opt.add("lastname,l",
												"The lastname of the user",
												CONFIG,
												lastname);

				opt.add("email,m",
												"The email of the user",
												CONFIG,
												email);
										

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
		}
		
		if(opt.count("firstname")){

			cout << "The firstname is " << firstname << endl;
			
			newUser.setFirstname(firstname);
                        
		}


		if (opt.count("lastname")){
			
			cout <<"The lastname is " << lastname << endl;
			
			newUser.setLastname(lastname);

		}
  
		if (opt.count("email")){
	
			cout <<"The email is " << email << endl;
			
			newUser.setEmail(email);
		}

            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;
               
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}

		 
		if (( reqParam < UUPARAM )|| (opt.count("help"))){

			cout << "Usage: " << av[0] <<" [options] userId "<<endl;

			cout << opt << endl;
			
			return 0;
			
		}


/************** Call UMS connect service *******************************/

               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
							std::string sessionKey;
		
							int res = updateUser(sessionKey,newUser);


		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


