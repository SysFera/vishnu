

#include "listOptions.hh"
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
        
		std::string optionName;
	
		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListOptionsValues lsOptionsValues;
		
		UMS_Data::ListOptOptions lsOptions;

	


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
				
        opt.add("listAllDeftValue,a", 
						            "is an option for listing all default option values\n"
												"defined by VISHNU administrator",
												CONFIG);

				opt.add("userId,u",
		                 "an admin option for listing commands launched\n" 
										 "by a specific user identified by his/her userId",
										 CONFIG,
										 userId);

				opt.add("optionName,n",
									    	"is an option for listing all default option values\n"
												"defined by VISHNU administrator",
												CONFIG,
												optionName);
				
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


		if (opt.count("listAllDeftValue")){
			
			cout <<"It is an admin list option " << endl;
			
			lsOptions.setListAllDeftValue(true);
		}
			
		
		if (opt.count("userId")){
			
			cout <<"The user identifier is " << userId << endl;
			
			lsOptions.setUserId(userId);
			
		}
		
		if(opt.count("optionName")){
			
			cout <<"the optionName is : " << optionName << endl;
			
			lsOptions.setOptionName(optionName);
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

    
		
							int res = listOptions(sessionKey,lsOptionsValues,lsOptions);


							// Display the list
     printf("Name            Value          \n");
     printf("--------------- ---------------\n");
     //Imprimer la liste formatee
     for(int i = 0; i < lsOptionsValues.getOptionValues().size(); i++) {
        printOptionValue(lsOptionsValues.getOptionValues().get(i));
     }

	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


