

#include "configureOption.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){
     
       

	try {


		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

		int reqParam=0;   // to count the required parameters for the command
		/******* Parsed value containers ****************/
       
		string dietConfig;

		std::string optionName;
        
		std::string value;
		
		std::string sessionKey;


		/********** EMF data ************/


  		UMS_Data::OptionValue optionValue;



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
				
				opt.add("optionName",
		                 "the name of an option", 
										 HIDDEN,
										 optionName);

				opt.setPosition("optionName",1);


				opt.add("value",
								       "The value of an option",
												HIDDEN,
												value);

				opt.setPosition("value",1);

										
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


		
		if (opt.count("optionName")){
			
			cout <<"The option name is " << optionName << endl;
		
			optionValue.setOptionName(optionName);

			reqParam=reqParam+1;
			
		}
		
		if(opt.count("value")){
			
			cout <<"the value is : " << value << endl;
			
			optionValue.setValue(value);
			
			reqParam=reqParam+1;
		}
		
            
		if (opt.count("dietConfig")){
           
			cout <<"The diet config file " << dietConfig << endl;           
		}

		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;
			
			return 1;
		}

		if ((reqParam < COPARAM) || (opt.count("help"))){
			
			cout << "Usage: " << av[0] <<"  optionName value "<<endl;
				     
			cout << opt << endl;
							      
			return 0;
		}




/************** Call UMS connect service *******************************/

               // initializing DIET
							 
							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

		
							int res = configureOption(sessionKey,optionValue);
		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


