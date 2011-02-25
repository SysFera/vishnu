
#include "listLocalAccount.hh"
#include "utils.hh"
#include "listMachineUtils.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


		/******* Parsed value containers ****************/

		string dietConfig;

		std::string sessionKey;

		/********** EMF data ************/

		UMS_Data::ListLocalAccounts lsLocalAccount;

		UMS_Data::ListLocalAccOptions listOptions;

/******** Callback functions ******************/

		          boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListLocalAccOptions::setUserId,boost::ref(listOptions),_1));

							boost::function1<void,string> fMachineId( boost::bind(&UMS_Data::ListLocalAccOptions::setMachineId,boost::ref(listOptions),_1));

/**************** Describe options *************/


							boost::shared_ptr<Options> opt= makeListMachineOptions(av[0],fUserId, dietConfig, fMachineId);


							opt->add("adminListOption,a",
						            "is an admin option for listing all local configurations of all users	",
												CONFIG);


	try {
/**************  Parse to retrieve option values  ********************/

		opt->parse_cli(ac,av);

		bool isEmpty=opt->empty();//if no value was given in the command line

		opt->parse_env(env_name_mapper());

		opt->notify();


/********  Process **************************/

		if (opt->count("adminListOption")){

			cout <<"It is an admin list option " << endl;

			listOptions.setAdminListOption(true);
		}

		checkVishnuConfig(*opt);

		if ( opt->count("help")){

			helpUsage(*opt," [options]");

			return 0;
		}


/************** Call UMS connect service *******************************/


               // initializing DIET

							  if (diet_initialize(dietConfig.c_str(), ac, av)) {
                    cerr << "DIET initialization failed !" << endl;
               return 1;
              }


 // get the sessionKey

                sessionKey=getLastSessionKey(getppid());

               if(false==sessionKey.empty()){

               cout <<"the current sessionkey is: " << sessionKey <<endl;
							
                listLocalAccount(sessionKey,lsLocalAccount,listOptions);


               }


							// Display the list
      if(isEmpty) {
          cout << lsLocalAccount << endl;
      }
      else {
             for(int i = 0; i < lsLocalAccount.getAccounts().size(); i++) {
               cout << lsLocalAccount.getAccounts().get(i) ;
             }
      }

	}// End of try bloc

	catch(VishnuException& e){// catch all Vishnu runtime error

		errorUsage(av[0], e.getMsg(),EXECERROR);

		return e.getMsgI() ;

	}

	catch(std::exception& e){
		errorUsage(av[0], e.what());

		return 1;
	}

	return 0;

}// end of main


