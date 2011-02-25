#include "configureDefaultOption.hh"
#include "configureOptionUtils.hpp"
#include "sessionUtils.hpp"
#include "utils.hh"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

int main (int ac, char* av[]){


		/******* Parsed value containers ****************/

		string dietConfig;

		std::string sessionKey;

		/********** EMF data ************/

  		UMS_Data::OptionValue optionValue;

			/******** Callback functions ******************/


			      boost::function1<void,string> fOptionName( boost::bind(&UMS_Data::OptionValue::setOptionName,boost::ref(optionValue),_1));

						boost::function1<void,string> fValue( boost::bind(&UMS_Data::OptionValue::setValue,boost::ref(optionValue),_1));

/**************** Describe options *************/

  boost::shared_ptr<Options> opt= makeConfigureOptions(av[0],dietConfig,fOptionName, fValue);


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


 // get the sessionKey


                sessionKey=getLastSessionKey(getppid());


               if(false==sessionKey.empty()){


               cout <<"the current sessionkey is: " << sessionKey <<endl;


                configureDefaultOption(sessionKey,optionValue);


               }



  }// End of try bloc

 catch(po::required_option& e){// a required parameter is missing

           usage(*opt,"[options] optionName value ","required parameter is missing");
  }
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


