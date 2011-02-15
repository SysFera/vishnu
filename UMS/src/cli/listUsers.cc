

#include "listUsers.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";


		/******* Parsed value containers ****************/
       
		string dietConfig;
		
		std::string userIdOption;
	
		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListUsers lsUsers;
		


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

				opt.add("userIdOption,i",
										 "An option for listing all default option values\n"
										 "defined by VISHNU administrator",
										 CONFIG,
										 userIdOption);
				
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
			
		
		if (opt.count("userIdOption")){
			
			cout <<"The user identifier option is " << userIdOption << endl;
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

    
		
							int res = listUsers(sessionKey,lsUsers, userIdOption);


							// Display the list
      if(isEmpty) {
         cout << lsUsers << endl;
      }
      else {
         for(int i = 0; i < lsUsers.getUsers().size(); i++) {
           cout << lsUsers.getUsers().get(i) << endl;
         }
      }

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


