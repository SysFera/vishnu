#include "close.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	try {

		Configuration config(av[0]);// take the command line name

		string defaultConfig = "VishnuConfig.cfg";

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
  
		opt.add("version,v", "print version message",GENERIC);

		opt.add<string >("sessionKey,s","The session Key",ENV);

/**************  Parse to retrieve option values  ********************/
 
		opt.parse_cli(ac,av);

		//opt.parse_cfile();
		
		opt.parse_env(env_name_mapper());

		opt.notify();





/********  Process **************************/

		if (opt.count("help")){

			cout << "Usage: " << av[0] << endl;

		}

		
		if (opt.count("sessionKey")){
			
			cout <<"your session key is " << opt.get<string>("sessionKey") << endl;
		}	
			


/************** Call UMS connect service *******************************/

		//int res=VISHNU::UMS::close(sessionKey);


		

	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}
	
	return 0;

}// end of main


