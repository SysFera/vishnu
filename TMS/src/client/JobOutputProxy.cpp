#include <string>
#include <iostream>
#include "JobOutputProxy.hpp"
#include "utilClient.hpp"
#include "DIET_client.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "utilVishnu.hpp"
#include "api_fms.hpp"
#include "tmsClientUtils.hpp"

namespace bfs = boost::filesystem;
using namespace vishnu;
using namespace std;

/**
 * \param session The object which encapsulates the session information
 * \param machine The object which encapsulates the machine information
 * \param outDir The output directory where the files will be stored
 * (default is current directory)
 * \brief Constructor
 */
JobOutputProxy::JobOutputProxy( const SessionProxy& session,
		const std::string& machineId,
		const std::string& outDir)
:msessionProxy(session), mmachineId(machineId), moutDir(outDir) {
}

/**
 * \brief Function to get the job results
 * \param jobId The Id of the
 * \return The job results data structure
 */
//TMS_Data::JobResult_ptr
TMS_Data::JobResult
JobOutputProxy::getJobOutPut(const std::string& jobId) {

	std::string serviceName = "jobOutputGetResult@";
	serviceName.append(mmachineId);

	diet_profile_t* getJobOutPutProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
	string sessionKey = msessionProxy.getSessionKey();
	std::string msgErrorDiet = "call of function diet_string_set is rejected ";

	//IN Parameters
	if ( diet_string_set(diet_parameter(getJobOutPutProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE) ) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}
	if ( diet_string_set(diet_parameter(getJobOutPutProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE) ) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}
	TMS_Data::JobResult jobResult; jobResult.setJobId(jobId);
	::ecorecpp::serializer::serializer _ser;
	string jobResultToString =  _ser.serialize_str(const_cast<TMS_Data::JobResult_ptr>(&jobResult));
	if ( diet_string_set(diet_parameter(getJobOutPutProfile,2), const_cast<char*>(jobResultToString.c_str()), DIET_VOLATILE) ) {
		msgErrorDiet += "with the job result parameter " + jobResultToString;
		raiseDietMsgException(msgErrorDiet);
	}
	if (diet_string_set(diet_parameter(getJobOutPutProfile,3), const_cast<char*>(moutDir.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with outDir parameter "+moutDir;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameter
	diet_string_set(diet_parameter(getJobOutPutProfile,4), NULL, DIET_PERSISTENT_RETURN);

	//Call the Server
	if( diet_call(getJobOutPutProfile) ) {
		raiseDietMsgException("DIET call failure");
	}

	char* routputInfo=NULL;
	if( diet_string_get(diet_parameter(getJobOutPutProfile,4), &routputInfo, NULL) ){
		msgErrorDiet += " by receiving outputInfo";
		raiseDietMsgException(msgErrorDiet);
	}
	if( ! boost::starts_with(routputInfo, "/") ) {
		raiseExceptionIfNotEmptyMsg(routputInfo) ;
	}

	if( moutDir.size()==0 ) {
		moutDir = (bfs::path(bfs::current_path().string())).string() + "/DOWNLOAD_" + jobId  + vishnu::createSuffixFromCurTime();
		vishnu::createOutputDir(moutDir);
	}
	jobResult.setOutputDir(moutDir) ;

	CpFileOptions copts;
	copts.setIsRecursive(false) ;
	copts.setTrCommand(0); // for using scp
	string outputInfo = genericFileCopier(sessionKey, mmachineId, routputInfo, "", "/tmp", copts);

	string line = "" ;
	istringstream fdescStream (vishnu::get_file_content(routputInfo, false)) ;
	if(! getline(fdescStream, line)) line = "";

	boost::trim(line);
	ListStrings lineVec ;
	getline(fdescStream, line) ; boost::trim(line) ;
	boost::split(lineVec, line, boost::is_any_of(" ")) ;

	int nbFiles = lineVec.size() ;
	if( nbFiles > 0 && line.size() >0 ) {
		copyFiles(sessionKey, mmachineId, lineVec, moutDir, copts, 0) ;
		jobResult.setOutputPath(moutDir+"/"+lineVec[0]) ;
		if(nbFiles == 1) {
			jobResult.setErrorPath(moutDir+"/"+lineVec[0]) ;
		} else {
			jobResult.setErrorPath(moutDir+"/"+lineVec[1]) ;
		}
	}

	if(! getline(fdescStream, line)) line = "";
	vishnu::saveMissingFiles(moutDir+"/MISSING", line) ;

	diet_profile_free(getJobOutPutProfile);
	return jobResult;
}

/**
 * \brief Function to get the results of all job submitted
 * \return The list of the job results
 */
TMS_Data::ListJobResults_ptr
JobOutputProxy::getCompletedJobsOutput() {
	diet_profile_t* getCompletedJobsOutputProfile = NULL;
	std::string sessionKey;
	char* errorInfo = NULL;
	TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

	std::string serviceName = "jobOutputGetCompletedJobs@";
	serviceName.append(mmachineId);

	getCompletedJobsOutputProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 5);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,2), const_cast<char*>(moutDir.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with outDir parameter "+moutDir;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(getCompletedJobsOutputProfile,3), NULL, DIET_VOLATILE);

	//Call the Server
	if( diet_call(getCompletedJobsOutputProfile)) {
		raiseDietMsgException("DIET call failure");
	}

	char* routputInfo=NULL;
	if( diet_string_get(diet_parameter(getCompletedJobsOutputProfile,3), &routputInfo, NULL) ){
		msgErrorDiet += " by receiving outputInfo";
		raiseDietMsgException(msgErrorDiet);
	}
	if( ! boost::starts_with(routputInfo, "/") ) {
		raiseExceptionIfNotEmptyMsg(routputInfo) ;
	}

	char* listJobResultInString = NULL;
	if(diet_string_get(diet_parameter(getCompletedJobsOutputProfile,4), &listJobResultInString, NULL)){
		msgErrorDiet += " by receiving User serialized  message";
		raiseDietMsgException(msgErrorDiet);
	}
	parseEmfObject(std::string(listJobResultInString), listJobResults_ptr); /*To build the listJobResults_ptr */


	CpFileOptions copts;
	copts.setIsRecursive(false) ;
	copts.setTrCommand(0); // for using scp

	string line ;
	ListStrings lineVec ;
	istringstream fdescStream (vishnu::get_file_content(routputInfo, false)) ;
	int numJob = 0 ;
	while( getline(fdescStream, line) ) {
		boost::trim(line) ;
		boost::split(lineVec, line, boost::is_any_of(" ")) ;
		moutDir = (bfs::path(bfs::current_path().string())).string() + "/DOWNLOAD_" + lineVec[0] + vishnu::createSuffixFromCurTime() ;
		vishnu::createOutputDir(moutDir);
		listJobResults_ptr->getResults().get(numJob++)->setOutputDir(moutDir);
		copyFiles(sessionKey, mmachineId, lineVec, moutDir, copts, 1) ;
		if( !getline(fdescStream, line)) break ;
		vishnu::saveMissingFiles(moutDir+"/MISSING", line) ;
	}

	diet_profile_free(getCompletedJobsOutputProfile);
	return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
