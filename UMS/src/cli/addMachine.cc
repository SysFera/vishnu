

#include "addMachine.hh"

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
				
				opt.add("name",
		                 "The name of the machine",
										 HIDDEN,
										 name);

				opt.setPosition("name",1);


				opt.add("site",
								"The location of the machine",
								HIDDEN,
								site);

				opt.setPosition("site",1);

				opt.add("language",
												"The language in which the description of the machine has been done",
												HIDDEN,
												language);

				opt.setPosition("language",1);


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

		if (opt.count("name")){
			
			cout <<"The name of the machine is " << name << endl;
			
			newMachine.setName(name);
			
			reqParam=reqParam+1;
			
		}
		
		if(opt.count("site")){
			
			cout <<"the site is : " << site << endl;
			
			newMachine.setSite(site);
			
			reqParam=reqParam+1;
		}
		
		if(opt.count("language")){

			cout << "The language is " << language << endl;
			
			newMachine.setLanguage(language);
			
			reqParam=reqParam+1;
                        
		}

            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}


		if ((reqParam < AMPARAM) || (opt.count("help"))){
			
			cout << "Usage: " << av[0] <<" name site language "<<endl;
				     
			cout << opt << endl;
							      
			return 0;
		}
		else{//Fix me

		cout << "Enter the Machine Description:\n ";

		cin >> machineDescription;
			
		newMachine.setMachineDescription(machineDescription);

		}




/************** Call UMS connect service *******************************/

               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

    
		
							int res = addMachine(sessionKey,newMachine);



	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


