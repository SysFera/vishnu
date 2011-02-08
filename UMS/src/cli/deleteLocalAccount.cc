

#include "deleteLocalAccount.hh"

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
		
		std::string sessionKey;


		/********** EMF data ************/





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
		
        opt.add<string>("dietConfig,c", 
						            "The diet config file",
												ENV,
												dietConfig);
				
				opt.add<string>("userId",
		                 "the Vishnu user identifier of the user of the local user configuration",
										 HIDDEN,
										 userId);

				opt.setPosition("userId",1);


				opt.add<string>("machineId",
									    	"the identifier of the machine associated to the local user configuration",
												HIDDEN,
												machineId);

				opt.setPosition("machineId",1);



										
				opt.add<string>("sessionKey",
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
			
			reqParam=reqParam+1;
			
		}
		
		if(opt.count("machineId")){
			
			cout <<"the machineId is : " << machineId << endl;
			
			reqParam=reqParam+1;
		}
		
            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}


		if ((reqParam < DLAPARAM) || (opt.count("help"))){
			
			cout << "Usage: " << av[0] <<"  userId machineId "<<endl;

				     
			cout << opt << endl;

							      
			return 0;
		}




/************** Call UMS connect service *******************************/

 /*              // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = deleteLocalAccount(sessionKey,userId,machineId);


	*/	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


