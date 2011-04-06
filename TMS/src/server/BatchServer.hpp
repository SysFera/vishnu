#ifndef TMS_BATCH_SERVER_H 
#define TMS_BATCH_SERVER_H

#include <string>
#include <iostream>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"


class BatchServer
{
public:

  BatchServer();
   
  virtual int submit(const char* scriptPath, const TMS_Data::SubmitOptions& options, TMS_Data::Job& job)=0;
	virtual int cancel()=0;
	virtual TMS_Data::Job getJob()=0;
	virtual TMS_Data::ListJobs* listJobs()=0;
	virtual TMS_Data::JobResult getJobResults()=0;
	virtual TMS_Data::ListQueues* listQueues()=0;
	virtual TMS_Data::ListJobResults* getAllJobsResults()=0;

  virtual ~BatchServer();
};

#endif
