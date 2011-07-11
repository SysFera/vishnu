/**
 * \file get_update_frequency.cpp
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
#include "displayer.hpp"

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct GetUpDateFreqFunc {

  GetUpDateFreqFunc(){};

  int operator()(std::string sessionKey) {
    int freq;
    int res = getUpdateFrequency(sessionKey, freq);
    displayFreq(freq);
    return res;
  }
};

int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  GetUpDateFreqFunc getUpDateFreqFunc;
  return GenericCli().run(getUpDateFreqFunc, dietConfig, argc, argv);
}
