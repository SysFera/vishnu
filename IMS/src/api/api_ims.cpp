#include "api_ims.hpp"

int
exportCommands(const string sessionKey,
	       const string oldSessionId,
	       string filename,
	       IMS_Data::ExportOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}

int
getMetricCurrentValue(const string sessionKey,
		      string machineId,
		      IMS_Data::Metric val,
		      IMS_Data::CurMetricOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}

int
getMetricHistory(const string sessionKey,
		 string machineId,
		 IMS_Data::MetricType type,
		 IMS_Data::ListMetric list,
		 IMS_Data::MetricHistOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
getProcesses(const string sessionKey,
	     IMS_Data::ListProcesses list,
	     IMS_Data::ProcessOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}



int
setSystemInfo(const string sessionKey,
	      IMS_Data::SystemInfo sys)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int 
setSystemThreshold(const int sessionKey,
		   IMS_Data::Threshold threshold)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int 
setSystemThreshold(const int sessionKey,
		   IMS_Data::ListThreshold list,
		   IMS_Data::ThresholdOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
defineUserIdentifier(const int sessionKey,
		     string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
defineMachineIdentifier(const int sessionKey,
			string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
defineJobIdentifier(const int sessionKey,
		    string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
defineTransferIdentifier(const int sessionKey,
			 string fmt)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
loadShed(const string sessionKey,
	 string machineId,
	 IMS_Data::LoadShedType)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
setUpdateFrequency(const string sessionKey,
		   int freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
getUpdateFrequency(const string sessionKey,
		   int freq)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
stop(const string sessionKey,
     IMS_Data::Process proc)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
getSystemInfo(const string sessionKey,
	      IMS_Data::ListSysInfo list,
	      IMS_Data::SysInfoOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}


int
restart(const string sessionKey,
	string machineId,
	IMS_Data::RestartOp op)
  throw (UMSVishnuException, IMSVishnuException, UserException, SystemException){
  return IMS_SUCCESS;
}

