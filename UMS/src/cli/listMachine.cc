

#include "listMachine.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";


		/******* Parsed value containers ****************/
       
		string dietConfig;
		
		std::string userId;
        
		std::string machineId;
		
		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListMachines lsMachine;
		
		UMS_Data::ListMachineOptions listOptions;

	


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
						             "An admin option for listing machines in which a specific user has a local configuration",
												CONFIG,
												userId);

				opt.add("listAllmachine,a",
		                    "An option for listing all VISHNU machines", 
										    CONFIG);

				opt.add("machineId,i",
									    	"An option for listing information about a	specific machine",
												CONFIG,
												machineId);
										
				opt.add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		bool isEmpty=opt.empty();//if no value was given in the command line

		opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/


		if (opt.count("listAllmachine")){
			
			cout <<"We nedd to list all VISHNU machines " << endl;
			
			listOptions.setListAllmachine(true);
		}
			
		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
			
			listOptions.setUserId(userId);
			
		}
		
		if(opt.count("machineId")){
			
			cout <<"the machineId is : " << machineId << endl;
			
			listOptions.setMachineId(machineId);
		}
		
		if(opt.count("sessionKey")){
			
			cout <<"the session key is : " << sessionKey << endl;
		}
            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}


		if ( opt.count("help")){
			
			cout << "Usage: " << av[0] <<" [options]  "<<endl;
			
			cout << opt << endl;
							      
			return 0;
		}




/************** Call UMS connect service *******************************/

		
               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = listMachine(sessionKey,lsMachine,listOptions);

      							// Display the list
     if(isEmpty) {
         cout << lsMachine << endl;
     } 
     else {
         for(int i = 0; i < lsMachine.getMachines().size(); i++) {
            cout << lsMachine.getMachines().get(i) << endl;
          }
     }	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


