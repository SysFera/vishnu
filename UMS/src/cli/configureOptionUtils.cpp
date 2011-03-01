#include "configureOptionUtils.hpp"
#include "Options.hpp"
#include "sessionUtils.hpp"
#include "utils.hpp"
#include <boost/bind.hpp>

void makeConfigureOptions(boost::shared_ptr<Options> opt,std::string& dietConfig,
                          StringcallBackType& fOptionName,StringcallBackType& fValue){



  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);



  opt->add("optionName",
           "the name of an option",
           HIDDEN,
           fOptionName,
           1);

  opt->setPosition("optionName",1);


  opt->add("value",
           "The value of an option",
           HIDDEN,
           fValue,
           1);

  opt->setPosition("value",1);


}


void commonConfigure(boost::shared_ptr<Options> opt, int ac, char* av[], const ConfigureCallBackType& conf_func){


  /******* Parsed value containers ****************/

  std::string dietConfig;

  std::string sessionKey;


  /********** EMF data ************/


  UMS_Data::OptionValue optionValue;

  /******** Callback functions ******************/


  boost::function1<void,std::string> fOptionName( boost::bind(&UMS_Data::OptionValue::setOptionName,boost::ref(optionValue),_1));
  boost::function1<void,std::string> fValue( boost::bind(&UMS_Data::OptionValue::setValue,boost::ref(optionValue),_1));

  /**************** Describe options *************/

  makeConfigureOptions(opt,dietConfig,fOptionName, fValue);



  /**************  Parse to retrieve option values  ********************/

  opt->parse_cli(ac,av);

  opt->parse_env(env_name_mapper());

  opt->notify();


  /********  Process **************************/

  checkVishnuConfig(*opt);

  /************** Call UMS connect service *******************************/

  // initializing DIET

  if (vishnuInitialize(const_cast<char*>(const_cast<char*>(dietConfig.c_str())), ac, av)) {
    std::cerr << "DIET initialization failed !" << std::endl;
    exit(EXIT_FAILURE);
  }


  // get the sessionKey


  /********************************************/


  sessionKey=getLastSessionKey(getppid());


  if(false==sessionKey.empty()){

    conf_func(sessionKey,optionValue);


  }

}




