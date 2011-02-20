

#include "deleteMachine.hh"
#include "utils.hh"

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){

		/******* Parsed value containers ****************/

		string dietConfig;

		std::string machineId;

		std::string sessionKey;


/**************** Describe options *************/

		Options opt(av[0] );

        opt.add("dietConfig,c",
						            "The diet config file",
												ENV,
												dietConfig);

				opt.add("machineId",
											"The identifier of the machine",
												HIDDEN,
												machineId,1);

				opt.setPosition("machineId",-1);

				opt.add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

	try {
/**************  Parse to retrieve option values  ********************/

		opt.parse_cli(ac,av);

		opt.parse_env(env_name_mapper());

		opt.notify();



/********  Process **************************/

		if (opt.count("machineId")){

			cout <<"The machine identifier is " << machineId << endl;

		}

		if (opt.count("dietConfig")){

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

							 deleteMachine(sessionKey,machineId);


	}// End of try bloc

   catch(po::required_option& e){// a required parameter is missing

          usage(opt," machineId ","required parameter is missing");
        }

        catch(std::exception& e){

          errorUsage(av[0],e.what());

          return 1;
  }

	return 0;

}// end of main


