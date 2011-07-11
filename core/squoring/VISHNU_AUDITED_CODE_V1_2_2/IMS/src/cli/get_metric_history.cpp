/**
 * \file get_metric_history.cpp
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

struct MetricHistoryFunc {

  std::string mmachineId;
  IMS_Data::MetricHistOp mop;
  boost::shared_ptr<Options> mcliOpt;
  std::string mstartTime;
  std::string mendTime;

  MetricHistoryFunc(const std::string& machineId, const IMS_Data::MetricHistOp& op,
                    boost::shared_ptr<Options> cliOpt, const std::string& startTime,
                    const  std::string& endTime):
   mmachineId(machineId), mop(op), mcliOpt(cliOpt), mstartTime(startTime), mendTime(endTime)
  {};

  int operator()(std::string sessionKey) {
    //convert the date in long format
    if(mcliOpt->count("start")){
      mop.setStartTime(convertLocaltimeINUTCtime(string_to_time_t(mstartTime)));
    }

    if(mcliOpt->count("end")){
      mop.setEndTime(convertLocaltimeINUTCtime(string_to_time_t(mendTime)));
    }

    IMS_Data::ListMetric met;
    int res = getMetricHistory(sessionKey, mmachineId, met, mop);
    displayListMetric(&met);
    return res;
  }
};


boost::shared_ptr<Options>
makeGHMOp(string pgName,
         std::string& startTime,
         std::string& endTime,
        boost::function1<void, IMS_Data::MetricType>& ftype,
	     string& dietConfig){
  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli options
  opt->add("start,s",
	   "The start time to get the history\n"
     "(in the format ""YYYY-MM-DD H:mm:ss"")",
	   CONFIG,
	   startTime);

  opt->add("end,e",
	   "The end time to get the history\n"
     "(in the format ""YYYY-MM-DD H:mm:ss"")",
	   CONFIG,
	   endTime);

  opt->add("type,t",
	   "The type of the metric to get",
	   CONFIG,
	   ftype);

  return opt;
}

int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string mid;
  string startTime;
  string endTime;

  /********** EMF data ************/
  IMS_Data::MetricHistOp op;

  /******** Callback functions ******************/
  boost::function1<void,IMS_Data::MetricType> ftype(boost::bind(&IMS_Data::MetricHistOp::setType,boost::ref(op),_1));

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeGHMOp(argv[0], startTime, endTime, ftype,  dietConfig);

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
  MetricHistoryFunc metricHistoryFunc(mid, op, opt, startTime, endTime);
  return GenericCli().run(metricHistoryFunc, dietConfig, argc, argv);
}
