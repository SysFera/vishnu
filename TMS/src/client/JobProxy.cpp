
#include <iostream>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilClient.hpp"
#include "api_fms.hpp"
#include "api_ums.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/find.hpp>

using namespace std;
namespace bfs=boost::filesystem;
typedef vector<string> ListStrings ;
/**
 * \param session The object which encapsulates the session information
 * \param machineId The id of the machine
 * \param job The job data structure
 * \brief Constructor
 */
JobProxy::JobProxy(const SessionProxy& session,
		const std::string & machineId,
		TMS_Data::Job& job)
:msessionProxy(session), mmachineId(machineId), mjob(job) {

}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */
int
JobProxy::submitJob(const std::string& scriptContent,
		const TMS_Data::SubmitOptions& options) {

	string serviceName = "jobSubmit_";
	serviceName.append(mmachineId);
	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	string sessionKey = msessionProxy.getSessionKey().c_str();

	diet_profile_t* submitJobProfile = diet_profile_alloc(serviceName.c_str(), 4, 4, 6);

	//IN Parameters
	if (diet_string_set(diet_parameter(submitJobProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(submitJobProfile,2), const_cast<char*>(scriptContent.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+scriptContent;
		raiseDietMsgException(msgErrorDiet);
	}

	ListStrings paramsVec ;
	boost::split(paramsVec, options.getFileParams(), boost::is_any_of(" ")) ;

	string rdestDir = bfs::unique_path("/tmp/fms%%%%%%").string();
	string fqdnDestDir = "";
	if( paramsVec.size() ) {
		fqdnDestDir = mmachineId + ":" +  rdestDir;
		if(vishnu::createDir(sessionKey, fqdnDestDir)){
			throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "unable to create the upload directory : "+fqdnDestDir);
		}
	}

	std::ostringstream paramsBuf ;
	for(ListStrings::const_iterator it = paramsVec.begin(); it != paramsVec.end(); it++){

		size_t pos = (*it).find("=") ; if(pos == std::string::npos) continue ; //*it would be in the form of param=path
		string param = (*it).substr(0, pos) ;
		string path = (*it).substr(pos+1, std::string::npos);

		if( ! bfs::exists(path) ) throw FMSVishnuException(ERRCODE_FILENOTFOUND, path);

		string rpath = rdestDir + "/" + bfs::path(path).filename().string();
		string fqdnPath = mmachineId + ":" +  + "/" + rpath;

		CpFileOptions copts;
		copts.setIsRecursive(true) ;
		copts.setTrCommand(0); // for using scp
		if( vishnu::copyFile(sessionKey, path, fqdnPath, copts) ) {

			UMS_Data::ListMachines machines;
			UMS_Data::ListMachineOptions mopts;
			mopts.setListAllMachine(false);
			mopts.setMachineId(mmachineId);
			if(vishnu::listMachines(sessionKey, machines, mopts) || !machines.getMachines().size()) {
				throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "unable to get the information concerning the machine "+mmachineId);
			}
			throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "error while copying the file " + path
					+" to "+fqdnPath+" (machine name : "+machines.getMachines().get(0)->getName()+ ")");
		}

		paramsBuf << ((paramsBuf.str().size() != 0)? " " : "") + param << "=" << rpath ;
	}

	TMS_Data::SubmitOptions& refOptions = const_cast<TMS_Data::SubmitOptions&>(options);
	refOptions.setFileParams(paramsBuf.str()) ;

	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	string optionsToString = _ser.serialize_str(const_cast<TMS_Data::SubmitOptions_ptr>(&refOptions));

	if (diet_string_set(diet_parameter(submitJobProfile,3), const_cast<char*>(optionsToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with optionsInString parameter "+std::string(optionsToString);
		raiseDietMsgException(msgErrorDiet);
	}

	_ser.resetSerializer();

	//To serialize the job object in to optionsInString
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob)).c_str();

	if (diet_string_set(diet_parameter(submitJobProfile,4), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(submitJobProfile,5), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(submitJobProfile,6), NULL, DIET_VOLATILE);

	char* jobInString = NULL;
	char* errorInfo = NULL;
	if(!diet_call(submitJobProfile)) {
		if(diet_string_get(diet_parameter(submitJobProfile,5), &jobInString, NULL)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(diet_parameter(submitJobProfile,6), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	}
	else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	TMS_Data::Job_ptr job_ptr = NULL;

	parseEmfObject(std::string(jobInString), job_ptr);

	mjob = *job_ptr;

	delete job_ptr;
	diet_profile_free(submitJobProfile);
	return 0;
}

/**
 * \brief Function to cancel job
 * \return raises an exception on error
 */
int
JobProxy::cancelJob() {

	diet_profile_t* cancelJobProfile = NULL;
	std::string sessionKey;
	char* errorInfo = NULL;
	std::string serviceName = "jobCancel_";
	serviceName.append(mmachineId);

	cancelJobProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 3);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(cancelJobProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(cancelJobProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the job object in to optionsInString
	string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(diet_parameter(cancelJobProfile,2), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+jobToString;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(cancelJobProfile,3), NULL, DIET_VOLATILE);

	if(!diet_call(cancelJobProfile)) {
		if(diet_string_get(diet_parameter(cancelJobProfile,3), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	}
	else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	diet_profile_free(cancelJobProfile);
	return 0;
}


/**
 * \brief Function to get job information
 * \param options the options to submit job
 * \return raises an exception on error
 */
TMS_Data::Job
JobProxy::getJobInfo() {

	diet_profile_t* getJobInfoProfile = NULL;
	std::string sessionKey;
	char* jobInString = NULL;
	char* errorInfo = NULL;
	std::string serviceName = "jobInfo_";
	serviceName.append(mmachineId);

	getJobInfoProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getJobInfoProfile,0), const_cast<char*>(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getJobInfoProfile,1), const_cast<char*>(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}


	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	std::string jobToString =  _ser.serialize_str(const_cast<TMS_Data::Job_ptr>(&mjob));

	if (diet_string_set(diet_parameter(getJobInfoProfile,2), const_cast<char*>(jobToString.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with jobInString parameter "+std::string(jobToString);
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(getJobInfoProfile,3), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(getJobInfoProfile,4), NULL, DIET_VOLATILE);

	if(!diet_call(getJobInfoProfile)) {
		if(diet_string_get(diet_parameter(getJobInfoProfile,3), &jobInString, NULL)){
			msgErrorDiet += " by receiving User serialized  message";
			raiseDietMsgException(msgErrorDiet);
		}
		if(diet_string_get(diet_parameter(getJobInfoProfile,4), &errorInfo, NULL)){
			msgErrorDiet += " by receiving errorInfo message";
			raiseDietMsgException(msgErrorDiet);
		}
	}
	else {
		raiseDietMsgException("DIET call failure");
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	TMS_Data::Job_ptr job_ptr = NULL;

	parseEmfObject(std::string(jobInString), job_ptr);

	mjob = *job_ptr;
	delete job_ptr;

	diet_profile_free(getJobInfoProfile);
	return mjob;
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job
JobProxy::getData() const{
	return mjob;
}

/**
 * \brief Destructor
 */
JobProxy::~JobProxy() {
}
