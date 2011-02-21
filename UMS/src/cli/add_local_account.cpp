#include "addLocalAccount.hh"
#include "utils.hh"
#include "localAccountUtils.hpp"
#include <boost/bind.hpp>
namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){





		/******* Parsed value containers ****************/

		string dietConfig;
    string sessionKey;

		/********** EMF data ************/

		UMS_Data::LocalAccount newAcLogin;

/******** Callback functions ******************/

					boost::function1<void,string> fUserId( boost::bind(&UMS_Data::LocalAccount::setUserId,boost::ref(newAcLogin),_1));
					boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::LocalAccount::setMachineId,boost::ref(newAcLogin),_1));
					boost::function1<void,string> fAcLogin( boost::bind(&UMS_Data::LocalAccount::setAcLogin,boost::ref(newAcLogin),_1));
					boost::function1<void,string> fSshKeyPath( boost::bind(&UMS_Data::LocalAccount::setSshKeyPath,boost::ref(newAcLogin),_1));
					boost::function1<void,string> fHomeDirectory( boost::bind(&UMS_Data::LocalAccount::setHomeDirectory,boost::ref(newAcLogin),_1));



		/*****************Out parameters***/

		std::string sshPublicKey;



/**************** Describe options *************/

		boost::shared_ptr<Options> opt=makeLocalAccountOptions(av[0], fUserId,dietConfig,fMachineId,
				                                               fAcLogin,fSshKeyPath,fHomeDirectory,1);

				opt->add("sessionKey",
												"The session key",
												ENV,
												sessionKey);

	try {
/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		opt->parse_env(env_name_mapper());

		opt->notify();





/********  Process **************************/


		checkVishnuConfig(*opt);

		/************** Call UMS connect service *******************************/

              // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }


							 addLocalAccount(sessionKey,newAcLogin,sshPublicKey);


	}// End of try bloc

catch(po::required_option& e){// a required parameter is missing


	usage(*opt," userId machineId acLogin sshKeyPath homeDirectory ","required parameter is missing");
  }
  catch(std::exception& e){

		errorUsage(av[0],e.what());

		return 1;
  }

  return 0;

}// end of main


