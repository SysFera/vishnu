/**
 * \file set_update_frequency.cpp
 * This file defines the VISHNU command to get the metric history
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct SetUpdateFreqFunc {

  std::string mfreq;

  SetUpdateFreqFunc(const std::string& freq):
   mfreq(freq)
  {};

  int operator()(std::string sessionKey) {
    return setUpdateFrequency(sessionKey, convertToInt(mfreq));
  }
};

int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string freq;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli obligatory parameters
  opt->add("freq,f",
	   "represents the id of the machine",
	   HIDDEN,
	   freq,1);
  opt->setPosition("freq",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "freq");

  //call of the api function
  SetUpdateFreqFunc setUpdateFreqFunc(freq);
  return GenericCli().run(setUpdateFreqFunc, dietConfig, argc, argv); 
}

