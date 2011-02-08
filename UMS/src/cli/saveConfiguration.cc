#include "saveConfiguration.hh"

#include "utils.hh" 

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	try {
                string filePath;
                string sessionKey;
                string dietConfig;

		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";
		
		int reqParam=0;   // to count the required parameters for the command


		/********** EMF Data ****************************/


  		UMS_Data::Configuration configuration;
    

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
						           "The session key",
											 ENV,
											 sessionKey);

		opt.add("filePath",
					  "The path of the file in which VISHNU configuration will be saved",	
						HIDDEN,
						filePath);

		opt.setPosition("filePath",-1);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		//opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/
		
		if (opt.count("filePath")){
			
			cout <<"The file Path is " << filePath << endl;

			reqParam=reqParam+1;
		}
		

		if (opt.count("dietConfig")){

                      
			cout <<"The diet config file " << dietConfig << endl;

		}

		else{
			
			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}


		if(opt.count("sessionKey")){

			cout << "The session Key is: " << sessionKey <<endl;

		}


		 if ((reqParam < SCPARAM)|| (opt.count("help"))){

			 cout << "Usage: " << av[0] <<" filePath"<<endl;
			
			 cout << opt << endl;
			
			 return 0;
		 }




/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {
                   
				  cerr << "DIET initialization failed !" << endl;
               
				  return 1;
              }
	
              saveConfiguration(sessionKey,filePath,configuration);


              //A enlever apres, c'est juste pour les tests
      //To set the file path 
      std::cout << "config filePath=" << configuration.getFilePath() << std::endl;
      //To set the user list 
      for(int i = 0; i < configuration.getListConfUsers().size(); i++) {
        UMS_Data::User_ptr user = configuration.getListConfUsers()[i];
        print(user, i);
      }
      //To set the machine list 
      for(int i = 0; i < configuration.getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = configuration.getListConfMachines().get(i);
        print(machine, i);
     }
     //To set the LocalAccounts list 
     for(int i = 0; i < configuration.getListConfLocalAccounts().size(); i++) {
       UMS_Data::LocalAccount_ptr localAccount = configuration.getListConfLocalAccounts().get(i);
       print(localAccount, i);
     }

	

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


