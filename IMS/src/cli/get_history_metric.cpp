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

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;


boost::shared_ptr<Options>
makeGHMOp(string pgName, 
	     boost::function1<void, long>& fstart,
	     boost::function1<void, long>& fend,
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
	   "The start time to get the history",
	   CONFIG,
	   fstart);

  opt->add("end,e",
	   "The end time to get the history",
	   CONFIG,
	   fend);

  opt->add("type,t",
	   "The end time to get the history",
	   CONFIG,
	   ftype);

  return opt;
}

int main (int argc, char* argv[]){
  
  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string mid;

  /********** EMF data ************/
  IMS_Data::MetricHistOp op;

  /******** Callback functions ******************/
  boost::function1<void,long> fstart(boost::bind(&IMS_Data::MetricHistOp::setStartTime,boost::ref(op),_1));
  boost::function1<void,long> fend(boost::bind(&IMS_Data::MetricHistOp::setEndTime,boost::ref(op),_1));
  boost::function1<void,IMS_Data::MetricType> ftype(boost::bind(&IMS_Data::MetricHistOp::setEndTime,boost::ref(op),_1));

  /*********** Out parameters *********************/
  IMS_Data::ListMetric met;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt = makeGHMOp(argv[0], fstart, fend, ftype,  dietConfig);

  // All cli obligatory parameters
  opt->add("machineId,m",
	   "represents the id of the machine",
	   HIDDEN,
	   mid,1);
  opt->setPosition("machineId",1);
 
  CLICmd cmd = CLICmd (argc, argv, opt, dietConfig);

  // Parse the cli and setting the options found
  ret = cmd.parse(env_name_mapper());

  if (ret != CLI_SUCCESS){
    helpUsage(*opt,"[options]");  
    return ret;
  }

  // PreProcess (adapt some parameters if necessary)
  checkVishnuConfig(*opt);  
  if ( opt->count("help")){
    helpUsage(*opt,"[options]");
    return 0;
  }

  // Process command
  try {
    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : get job output
    if(false==sessionKey.empty()){
      cout <<currentSessionKeyMsg << sessionKey <<endl;
      getMetricHistory(sessionKey, mid, met, op);
    }
    displayListMetric(&met);
  } catch(VishnuException& e){// catch all Vishnu runtime error
    std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
    errorUsage(argv[0], msg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(e)){
      removeBadSessionKeyFromFile(getppid());
    }
    return e.getMsgI() ;
  } catch(std::exception& e){// catch all std runtime error
    errorUsage(argv[0],e.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
