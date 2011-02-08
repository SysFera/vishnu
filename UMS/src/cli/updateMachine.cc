

#include "updateMachine.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

		int reqParam=0;   // to count the required parameters for the command
		/******* Parsed value containers ****************/
       
		string dietConfig;

		std::string name;
        
		std::string site;

		std::string language;
		
		std::string machineDescription; 
		
		std::string machineId; 
		
		std::string sessionKey;


		/********** EMF data ************/

		UMS_Data::Machine newMachine;




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
				
				opt.add("name,n",
		                 "The name of the machine",
										 CONFIG,
										 name);

				opt.add("site,s",
								"The location of the machine",
								CONFIG,
								site);

				opt.add("language,l",
												"The language in which the description of the machine has been done",
												CONFIG,
												language);

				opt.add("machineDescription,d",
											"The description of the machine, it is an interactive option",	
												CONFIG);

				opt.add("machineId",
											"The identifier of the machine",	
												HIDDEN,
												machineId);

				opt.setPosition("machineId",-1);

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

		if (opt.count("machineId")){
			
			cout <<"The machine identifier is " << machineId << endl;
			
			newMachine.setMachineId(machineId);
			
			reqParam=reqParam+1;
		}
		
		if (opt.count("name")){
			
			cout <<"The name of the machine is " << name << endl;
			
			newMachine.setName(name);
		}

		if(opt.count("site")){
			
			cout <<"the site is : " << site << endl;
			
			newMachine.setSite(site);
			
		}
		
		if(opt.count("language")){

			cout << "The language is " << language << endl;
			
			newMachine.setLanguage(language);
                        
		}

		if(opt.count("machineDescription")){//Fix me

		cout << "Enter the Machine Description:\n ";

		cin >> machineDescription;
			
		newMachine.setMachineDescription(machineDescription);

		}
            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}


		if ((reqParam < UMPARAM) || (opt.count("help"))){
			
			cout << "Usage: " << av[0] <<" [options] machineId "<<endl;
				     
			cout << opt << endl;
							      
			return 0;
		}





/************** Call UMS connect service *******************************/

               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = updateMachine(sessionKey,newMachine);


		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


