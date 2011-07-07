/**
 * \file set_system_threshold.cpp
 * This file defines the VISHNU command to set the system threshold
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
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

struct SetSysThresholdFunc {

  IMS_Data::Threshold msystemThreshold;

  SetSysThresholdFunc(const IMS_Data::Threshold& systemThreshold):
   msystemThreshold(systemThreshold)
  {};

  int operator()(std::string sessionKey) {
    return setSystemThreshold(sessionKey, msystemThreshold);
  }
};


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string handler;
  int value;
  IMS_Data::MetricType type;
   /********** EMF data ************/
  IMS_Data::Threshold systemThreshold;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  //Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("machineId,i",
            "represents the id of the machine",
            HIDDEN,
            machineId,1);
  opt->setPosition("machineId",1);

  opt->add("value,v",
            "represents the value of the threshold",
            HIDDEN,
            value,1);
  opt->setPosition("value",1);

  opt->add("type,t",
            "represents the type of the threshold",
            HIDDEN,
            type,1);
  opt->setPosition("type",1);

  opt->add( "handler,u",
            "represents the userId of the administrator that handles the threshold",
            HIDDEN,
            handler,1);
  opt->setPosition("handler",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId value type handler");

  //To set the corresponding values to the object systemThreshold
  systemThreshold.setMachineId(machineId);
  systemThreshold.setValue(value);
  systemThreshold.setType(type);
  systemThreshold.setHandler(handler);
  //call of the api function
  SetSysThresholdFunc setSysThresholdFunc(systemThreshold);
  return GenericCli().run(setSysThresholdFunc, dietConfig, argc, argv);
}

