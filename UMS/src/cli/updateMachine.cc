

#include "updateMachine.hh"
#include "utils.hh"
#include "machineUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){




		/******* Parsed value containers ****************/

		string dietConfig;

		std::string name;

		std::string site;

		std::string language;

		std::string machineDescription;

		std::string machineId;

		std::string sessionKey;


		/********** EMF data ************/

		UMS_Data::Machine upMachine;



/**************** Describe options *************/

		boost::function1<void,string> fName( boost::bind(&UMS_Data::Machine::setName,boost::ref(upMachine),_1));

		boost::function1<void,string> fSite( boost::bind(&UMS_Data::Machine::setSite,boost::ref(upMachine),_1));

			boost::function1<void,string> fLanguage( boost::bind(&UMS_Data::Machine::setLanguage,boost::ref(upMachine),_1));


			boost::shared_ptr<Options> opt= makeMachineOptions(av[0], fName,dietConfig, fSite,fLanguage);

        opt->add("sessionKey",
                        "The session key",
                        ENV,
                        sessionKey);

				opt->add("machineId",
											"The identifier of the machine",
												HIDDEN,
												machineId,
												1);

				opt->setPosition("machineId",-1);

				opt->add("machineDescription,d",
											"The description of the machine",
												CONFIG,
												machineDescription);

        try{


/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();





/********  Process **************************/

		if (opt->count("machineId")){

			cout <<"The machine identifier is " << machineId << endl;

		}

		if (opt->count("name")){

			cout <<"The name of the machine is " <<  upMachine.getName() << endl;

		}

		if(opt->count("site")){

			cout <<"the site is : " << upMachine.getSite() << endl;

		}

		if(opt->count("language")){

			cout << "The language is " <<  upMachine.getMachineId() << endl;

		}

		if(opt->count("machineDescription")){//Fix me

		cout << "Enter the Machine Description:\n ";


		getline(cin, machineDescription);

		upMachine.setMachineDescription(machineDescription);

		}

		if (opt->count("dietConfig")){

			cout <<"The diet config file " << dietConfig << endl;
		}
		else{

			cerr << "Set the VISHNU_CONFIG_FILE in your environment variable" <<endl;

			return 1;
		}


/************** Call UMS connect service *******************************/

               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }

							 updateMachine(sessionKey,upMachine);

	}// End of try bloc

				catch(po::required_option& e){// a required parameter is missing

					usage(*opt," machineId ","required parameter is missing");
				}

				catch(std::exception& e){

					errorUsage(av[0],e.what());

					return 1;
  }

  return 0;

}// end of main


