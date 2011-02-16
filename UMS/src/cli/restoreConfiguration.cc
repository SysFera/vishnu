#include "restoreConfiguration.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


	string filePath;

	string sessionKey;

	string dietConfig;

		int reqParam=0;   // to count the required parameters for the command

/**************** Describe options *************/


		Options opt(av[0] );

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
					  "The path of the VISHNU configuration file",
						HIDDEN,
						filePath);

		opt.setPosition("filePath",-1);


		try {

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


		 if ((reqParam < RCPARAM)|| (opt.count("help"))){

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

              restoreConfiguration(sessionKey, filePath);



	}// End of try bloc

	catch(std::exception& e){
		cout << e.what() <<endl;
		return 1;
	}

	return 0;

}// end of main


