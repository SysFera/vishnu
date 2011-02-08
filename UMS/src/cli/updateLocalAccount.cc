

#include "updateLocalAccount.hh"

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
        
		std::string machineId;

		std::string acLogin;
		
		std::string sshKeyPath;
		
		std::string sessionKey;

		std::string homeDirectory; 

		/********** EMF data ************/

		UMS_Data::LocalAccount newAcLogin;




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
				
				opt.add("userId",
		                 "the Vishnu user identifier of the user of the local user configuration",
										 HIDDEN,
										 userId);

				opt.setPosition("userId",1);


				opt.add("machineId",
									    	"the identifier of the machine associated to the local user configuration",
												HIDDEN,
												machineId);

				opt.setPosition("machineId",1);

				opt.add("acLogin,l",
												"login of the user on the associated machine",
												CONFIG,
												acLogin);

				opt.add("sshKeyPath,s",
												"The path of the ssh key of the user on the associated machine",
												CONFIG,
												sshKeyPath);

				opt.add("homeDirectory,d",
												"The path of the home directory of the user on the associated machine",
												CONFIG,
												homeDirectory);
										
				opt.add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/


		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
			
			newAcLogin.setUserId(userId);
			
			reqParam=reqParam+1;
			
		}
		
		if(opt.count("machineId")){
			
			cout <<"the machineId is : " << machineId << endl;
			
			newAcLogin.setMachineId(machineId);
			
			reqParam=reqParam+1;
		}
		
		if(opt.count("acLogin")){

			cout << "The acLogin is " << acLogin << endl;
			
			newAcLogin.setAcLogin(acLogin);
			
                        
		}


		if (opt.count("sshKeyPath")){
			
			cout <<"The sshKeyPath is " << sshKeyPath << endl;
			
			newAcLogin.setSshKeyPath(sshKeyPath);

		}
  
		if (opt.count("homeDirectory")){
	
			cout <<"The homeDirectory is " << homeDirectory << endl;
			
		newAcLogin.setHomeDirectory(homeDirectory);
			
		}

            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}


		if ((reqParam < ULAPARAM) || (opt.count("help"))){
			
			cout << "Usage: " << av[0] <<" [options] userId machineId "<<endl;

				     
			cout << opt << endl;

							      
			return 0;
		}




/************** Call UMS connect service *******************************/

               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = updateLocalAccount(sessionKey,newAcLogin);


	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


