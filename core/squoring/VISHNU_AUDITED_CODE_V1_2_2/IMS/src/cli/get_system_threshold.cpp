/**
 * \file get_system_threshold.cpp
 * This file defines the VISHNU command to get the system threshold
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
 */



#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>
#include "displayer.hpp"

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct GetSysThresholdFunc {

  IMS_Data::ThresholdOp mthresholdOp;

  GetSysThresholdFunc(const IMS_Data::ThresholdOp thresholdOp):
   mthresholdOp(thresholdOp)
  {};

  int operator()(std::string sessionKey) {
    IMS_Data::ListThreshold listThreshold;    
    int res = getSystemThreshold(sessionKey, listThreshold, mthresholdOp);
    displayListSysThreshold(&listThreshold);    
    return res;
  }
};

boost::shared_ptr<Options>
makeGetystemThresholdOpt(string pgName,
                  boost::function1<void, string>& fMachineId,
                  boost::function1<void, IMS_Data::MetricType>& fType,
                  string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("machineId,m",
            "represents the id of the machine",
            CONFIG,
            fMachineId);

  opt->add("metricType,t",
            "the type of the metric",
            CONFIG,
            fType);

  return opt;
}


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;

   /********** EMF data ************/
  IMS_Data::ThresholdOp thresholdOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fMachineId (boost::bind(&IMS_Data::ThresholdOp::setMachineId,boost::ref(thresholdOp),_1));
  boost::function1<void,IMS_Data::MetricType> fType (boost::bind(&IMS_Data::ThresholdOp::setMetricType,boost::ref(thresholdOp),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeGetystemThresholdOpt(argv[0], fMachineId, fType, dietConfig);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  GetSysThresholdFunc getSysThresholdFunc(thresholdOp);
  return GenericCli().run(getSysThresholdFunc, dietConfig, argc, argv);

}

