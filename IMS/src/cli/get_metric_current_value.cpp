/**
 * \file get_current_metric.cpp
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
#include <boost/bind.hpp>

#include "GenericCli.hpp"

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

struct MetricCurrentValueFunc {

  std::string mmachineId;
  IMS_Data::CurMetricOp mop;

  MetricCurrentValueFunc(const std::string& machineId, const IMS_Data::CurMetricOp& op):
   mmachineId(machineId), mop(op)
  {};

  int operator()(std::string sessionKey) {
    IMS_Data::ListMetric met;
    int res = getMetricCurrentValue(sessionKey, mmachineId, met, mop);
    displayListMetric(&met);
    return res;
  }
};


boost::shared_ptr<Options>
makeGCMOp(string pgName,
	  boost::function1<void, IMS_Data::MetricType>& ftype,
	  string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options

  opt->add("type,t",
	   "The type of the metric",
	   CONFIG,
	   ftype);

  return opt;
}

int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string mid;

  /********** EMF data ************/
  IMS_Data::CurMetricOp op;

  /******** Callback functions ******************/
  boost::function1<void,IMS_Data::MetricType> ftype(boost::bind(&IMS_Data::CurMetricOp::setMetricType,boost::ref(op),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeGCMOp(argv[0], ftype,  dietConfig);

  // All cli obligatory parameters
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   mid,1);
  opt->setPosition("machineId",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "machineId");

  //call of the api function
  MetricCurrentValueFunc metricMetricCurrentValueFunc(mid, op);
  return GenericCli().run(metricMetricCurrentValueFunc, dietConfig, argc, argv);

}
