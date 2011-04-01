#include <string>
#include <iostream>

#include "JobOutPutProxy.hpp"


/**
  * \param session The object which encapsulates the session information
  * \param machine The object which encapsulates the machine information
  * \param jobResult The job results data structure
  * \param listJobResults the list of job results data structure
  * \brief Constructor, raises an exception on error
  */
explicit
JobOutPutProxy( SessionProxy session,
                MachineProxy machine,
                TMS_Data::JobResult result,
                TMS_Data::ListJobResults* listJobResults)
:msessionProxy(session), mmachineProxy(machine), mjobResult(result), mlistJobResults(&listJobResults)
{
}


TMS_Data::JobResult
JobOutPutProxy::getJobOutPut(std::string jobId)
{
	return mjobResult;
}

TMS_Data::ListJobResults*
JobOutPutProxy::getAllJobsOutPut() {
	return mlistJobResults;
}

/**
  * \brief Destructor, raises an exception on error
  */

JobOutPutProxy::~JobOutPutProxy() {

}
