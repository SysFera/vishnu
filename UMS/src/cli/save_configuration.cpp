#include "saveConfiguration.hh"

#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

	string sessionKey;

	string dietConfig;

		/********** EMF Data ****************************/


  		UMS_Data::Configuration configuration;


/**************** Describe options *************/


		Options opt(av[0] );


        opt.add("dietConfig,c",
				        "The diet config file",
						ENV,
						dietConfig);

				opt.add("sessionKey,s",
						           "The session key",
											 ENV,
											 sessionKey);

	try {
/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		opt.parse_env(env_name_mapper());

		opt.notify();


/********  Process **************************/


		checkVishnuConfig(opt);

		if(opt.count("help")){

			helpUsage(opt,"[options]");

			 return 0;
		 }


/************** Call UMS connect service *******************************/


               // initializing DIET
              if (diet_initialize(dietConfig.c_str(), ac, av)) {

				  cerr << "DIET initialization failed !" << endl;

				  return 1;
              }

              saveConfiguration(sessionKey,configuration);


             //A enlever apres, c'est juste pour les tests
      for(int i = 0; i < configuration.getListConfUsers().size(); i++) {
        UMS_Data::User_ptr user = configuration.getListConfUsers()[i];
        cout << user;
      }
      //To set the machine list
      for(int i = 0; i < configuration.getListConfMachines().size(); i++) {
        UMS_Data::Machine_ptr machine = configuration.getListConfMachines().get(i);
        cout << machine;
     }
     //To set the LocalAccounts list
     for(int i = 0; i < configuration.getListConfLocalAccounts().size(); i++) {
       UMS_Data::LocalAccount_ptr localAccount = configuration.getListConfLocalAccounts().get(i);
       cout << localAccount;
     }



	}// End of try bloc

	catch(VishnuException& e){// catch all Vishnu runtime error

		errorUsage(av[0], e.getMsg(),EXECERROR);

		return e.getMsgI() ;

	}

	catch(std::exception& e){
		errorUsage(av[0],e.what());
		return 1;
	}

	return 0;

}// end of main


