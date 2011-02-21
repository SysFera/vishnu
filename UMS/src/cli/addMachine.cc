

#include "addMachine.hh"
#include "utils.hh"
#include "machineUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){




		/******* Parsed value containers ****************/

		string dietConfig;

		std::string machineDescription;

		std::string sessionKey;


		/********** EMF data ************/

		UMS_Data::Machine newMachine;


/**************** Callback functions *************/

		boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(newMachine),_1));
		boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(newMachine),_1));
		boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(newMachine),_1));


		boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage,1);

				opt->add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

				try{
/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();





/********  Process **************************/

		if (opt->count("name")){

			cout <<"The name of the machine is " << newMachine.getName() << endl;
		}

		if(opt->count("site")){

			cout <<"the site is : " << newMachine.getSite() << endl;

		}

		if(opt->count("language")){

			cout << "The language is " << newMachine.getLanguage() << endl;

		}


		if (opt->count("dietConfig")){

			cout <<"The diet config file " << dietConfig << endl;
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


		cout << "Enter the Machine Description:\n ";

		getline(cin, machineDescription);

		newMachine.setMachineDescription(machineDescription);





/************** Call UMS connect service *******************************/

               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

							 addMachine(sessionKey,newMachine);


	}// End of try bloc


				catch(po::required_option& e){// a required parameter is missing

					usage(*opt," name site language ","required parameter is missing");
  }
  catch(std::exception& e){

    errorUsage(av[0],e.what());

    return 1;
  }

  return 0;

}// end of main


