/**
 * \file list_options.cpp
 * This file defines the VISHNU list options command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "common.hpp"
#include "cliUtil.hpp"
#include "utils.hpp"
#include "utilVishnu.hpp"
#include "sessionUtils.hpp"
#include<boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct ListOptionsFunc {

  UMS_Data::ListOptionsValues mlsOptionsValues;

  UMS_Data::ListOptOptions mlsOptions;

  bool mfull;

  ListOptionsFunc(const UMS_Data::ListOptionsValues& lsOptionsValues,const UMS_Data::ListOptOptions& lsOptions , bool full):
   mlsOptionsValues(lsOptionsValues),mlsOptions(lsOptions), mfull(full)
  {};

  int operator()(std::string sessionKey) {
    int res = listOptions(sessionKey,mlsOptionsValues,mlsOptions);
    // Display the list
    if(mfull) {
      cout << mlsOptionsValues << endl;
    }
    else {
      for(unsigned int i = 0; i < mlsOptionsValues.getOptionValues().size(); i++) {
        cout <<mlsOptionsValues.getOptionValues().get(i) << endl;
      }
    }

    return res;
  }
};


int main (int ac, char* av[]){


  /******* Parsed value containers ****************/

  string dietConfig;


  /********** EMF data ************/

  UMS_Data::ListOptionsValues lsOptionsValues;

  UMS_Data::ListOptOptions lsOptions;

  /**************** Callback functions *************/

  boost::function1<void,string> fUserId( boost::bind(&UMS_Data::ListOptOptions::setUserId,boost::ref(lsOptions),_1));
  boost::function1<void,string> fOptionName( boost::bind(&UMS_Data::ListOptOptions::setOptionName,boost::ref(lsOptions),_1));

  /**************** Describe options *************/


  boost::shared_ptr<Options> opt(new Options (av[0]) );

  opt->add("dietConfig,c",
          "The diet config file",
          ENV,
          dietConfig);

  opt->add("listAllDeftValue,a",
          "is an option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG);

  opt->add("userId,u",
          "an admin option for listing commands launched\n"
          "by a specific user identified by his/her userId",
          CONFIG,
          fUserId);

  opt->add("optionName,n",
          "is an option for listing all default option values\n"
          "defined by VISHNU administrator",
          CONFIG,
          fOptionName);

  /*CLICmd cmd = CLICmd (ac, av, opt);

  try {
    opt->parse_cli(ac,av);
  } catch(po::error& e){ // catch all other bad parameter errors
    errorUsage(av[0], e.what());
    return CLI_ERROR_INVALID_PARAMETER;
  }

  bool isEmpty=opt->empty();//if no value was given in the command line

  // Parse the cli and setting the options found
  int ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[option]");
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);
  if ( opt->count("help")){
    helpUsage(*opt,"[option]");
    return 0;
  }*/

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, ac, av);

  /********  Process **************************/

  bool full = false;
  // Display the list
  if(isEmpty || opt->count("listAllDeftValue")) {
    full = true;
  }

  if( opt->count("listAllDeftValue")) {
    lsOptions.setListAllDeftValue (true);
  }
  ListOptionsFunc listFunc(lsOptionsValues,lsOptions, full);
  return GenericCli().run(listFunc, dietConfig, ac, av);
}// end of main



 
