/**
 * \file list_options.cpp
 * This file defines the VISHNU list options command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "sessionUtils.hpp"
#include<boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;

  std::string sessionKey;

  /********** EMF data ************/

  UMS_Data::ListOptionsValues lsOptionsValues;

  UMS_Data::ListOptOptions lsOptions;

  /**************** Callback functions *************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListOptOptions::setUserId,boost::ref(lsOptions),_1));
  boost::function1<void,string> fOptionName( boost::bind(&UMS_Data::ListOptOptions::setOptionName,boost::ref(lsOptions),_1));

  /**************** Describe options *************/


  Options opt(av[0] );

  opt.add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt.add("listAllDeftValue,a",
          "is an option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG);

  opt.add("userId,u",
          "an admin option for listing commands launched\n"
          "by a specific user identified by his/her userId",
          CONFIG,
          fUserId);

  opt.add("optionName,n",
          "is an option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG,
          fOptionName);


  try {
    /**************  Parse to retrieve option values  ********************/

    opt.parse_cli(ac,av);

    bool isEmpty=opt.empty();//if no value was given in the command line

    opt.parse_env(env_name_mapper());

    opt.notify();


    /********  Process **************************/

    if (opt.count("listAllDeftValue")){

      lsOptions.setListAllDeftValue(true);
    }


    checkVishnuConfig(opt);

    if ( opt.count("help")){

      helpUsage(opt,"[options] ");

      return 0;
    }

    /************** Call UMS list options service *******************************/

    // initializing DIET

    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {
    
    errorUsage(av[0],dietErrorMsg,EXECERROR);

          return  CLI_ERROR_DIET ;
    }



    // get the sessionKey

    sessionKey=getLastSessionKey(getppid());

    if(false==sessionKey.empty()){

      cout <<currentSessionKeyMsg << sessionKey <<endl;

      listOptions(sessionKey,lsOptionsValues,lsOptions);


    }


    // Display the list
    if(isEmpty || (opt.count("listAllDeftValue")) ) {
      cout << lsOptionsValues << endl;
    }
    else {
      for(unsigned int i = 0; i < lsOptionsValues.getOptionValues().size(); i++) {
        cout << lsOptionsValues.getOptionValues().get(i) << endl;
      }
    }

    printSuccessMessage();
  }// End of try bloc

  catch(po::error& e){ // catch all other bad parameter errors

    errorUsage(av[0], e.what());

    return CLI_ERROR_INVALID_PARAMETER;
  }

  catch(VishnuException& e){// catch all Vishnu runtime error

    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";

    errorUsage(av[0], msg,EXECERROR);

    return e.getMsgI() ;

  }

  catch(std::exception& e){// catch all std runtime error

    errorUsage(av[0],e.what());

    return CLI_ERROR_RUNTIME;
  }

  return 0;

}// end of main


