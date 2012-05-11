#include <string>
#include <iostream>
#include "JobOutputProxy.hpp"
#include "utilClient.hpp"
#include "DIET_Dagda.h"
#include "utilVishnu.hpp"
#include <omniORB4/CORBA.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace bfs = boost::filesystem;
using namespace vishnu;

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

	diet_profile_t* getJobOutPutProfile = NULL;
	std::string sessionKey;
	char* jobResultToString;
	char* errorInfo = NULL;
	char* IDContainer = NULL;
	diet_container_t content;
	char* outputPath = NULL;
	char* errorPath = NULL;

	TMS_Data::JobResult jobResult;

	jobResult.setJobId(jobId);

	std::string serviceName = "jobOutputGetResult_";
	serviceName.append(mmachineId);

	getJobOutPutProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getJobOutPutProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getJobOutPutProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	::ecorecpp::serializer::serializer _ser;
	//To serialize the options object in to optionsInString
	jobResultToString =  strdup(_ser.serialize_str(const_cast<TMS_Data::JobResult_ptr>(&jobResult)).c_str());

	if (diet_string_set(diet_parameter(getJobOutPutProfile,2), jobResultToString, DIET_VOLATILE)) {
		msgErrorDiet += "with the job result parameter "+std::string(jobResultToString);
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getJobOutPutProfile,3), strdup((moutDir.c_str())), DIET_VOLATILE)) {
		msgErrorDiet += "with outDir parameter "+moutDir;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(getJobOutPutProfile,4), NULL, DIET_VOLATILE);
	diet_container_set(diet_parameter(getJobOutPutProfile,5), DIET_PERSISTENT_RETURN);

	if( diet_call(getJobOutPutProfile) ) {
		raiseDietMsgException("DIET call failure");
	}

	if(diet_string_get(diet_parameter(getJobOutPutProfile,4), &errorInfo, NULL)){
		msgErrorDiet += " by receiving errorInfo message";
		raiseDietMsgException(msgErrorDiet);
	}

	/*To raise a vishnu exception if the receiving message is not empty*/
	raiseExceptionIfNotEmptyMsg(errorInfo);

	try {
		IDContainer = (getJobOutPutProfile->parameters[5]).desc.id;
		dagda_get_container(IDContainer);
		dagda_get_container_elements(IDContainer, &content);

		char* fName = NULL ;
		if (content.size != 0) {
			if( ! moutDir.size() ) {
				moutDir = (bfs::path(bfs::current_path().string())).string() + "/DOWNLOAD_" + jobId ;
				vishnu::createOutputDir(moutDir);
				jobResult.setOutputDir(moutDir) ;
			}
			//First get the description file
			dagda_get_file(content.elt_ids[0], &fName);
			std::string fDescStr = vishnu::get_file_content(fName) ;
			free(fName) ;

			std::cout << fDescStr << std::endl;
			ListStrings fDescVec ; boost::trim(fDescStr) ;
			boost::split(fDescVec, fDescStr, boost::is_any_of(" ")) ;

			if(content.size != fDescVec.size()) {
				throw UserException(ERRCODE_INVALID_PARAM, "DAGDA Exception: uncompleted list of transfered files");
			}

			for(int i = 1; i < content.size; i++) {
				dagda_get_file(content.elt_ids[i], &fName);
				vishnu::boostMoveFile(std::string(fName), moutDir, boost::trim_copy(fDescVec[i]));
				free(fName) ;
			}
			//Clean the container
			for(int i = 0; i < content.size; i++) {
				dagda_delete_data(content.elt_ids[i]);
			}
		}
	} catch (CORBA::Exception & e) {//To catch CORBA exception sent by DAGDA
		for(unsigned int i = 0; i < content.size; i++) {
			dagda_delete_data(content.elt_ids[i]);
		}
		dagda_delete_data(IDContainer);
		diet_profile_free(getJobOutPutProfile);
		throw UserException(ERRCODE_INVALID_PARAM,"CORBA Exception: "+ std::string(e._name())+
				" ("+std::string(IDContainer) + ")");
	}
	if (content.size == 2) {
		//To clean the container
		for (unsigned int i = 0; i < content.size; i++) {
			dagda_delete_data(content.elt_ids[i]);
		}
	}
	//To clean the container Id
	dagda_delete_data(IDContainer);
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
	char* listJobResultInString = NULL;
	char* errorInfo = NULL;
	TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

	char* IDContainer = NULL;
	diet_container_t content;

	std::string serviceName = "jobOutputGetCompletedJobs_";
	serviceName.append(mmachineId);

	getCompletedJobsOutputProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 5);
	sessionKey = msessionProxy.getSessionKey();

	std::string msgErrorDiet = "call of function diet_string_set is rejected ";
	//IN Parameters
	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with sessionKey parameter "+sessionKey;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
		msgErrorDiet += "with machineId parameter "+mmachineId;
		raiseDietMsgException(msgErrorDiet);
	}

	if (diet_string_set(diet_parameter(getCompletedJobsOutputProfile,2), strdup((moutDir.c_str())), DIET_VOLATILE)) {
		msgErrorDiet += "with outDir parameter "+moutDir;
		raiseDietMsgException(msgErrorDiet);
	}

	//OUT Parameters
	diet_string_set(diet_parameter(getCompletedJobsOutputProfile,3), NULL, DIET_VOLATILE);
	diet_string_set(diet_parameter(getCompletedJobsOutputProfile,4), NULL, DIET_VOLATILE);
	diet_container_set(diet_parameter(getCompletedJobsOutputProfile,5), DIET_PERSISTENT_RETURN);

	if( diet_call(getCompletedJobsOutputProfile)) {
		raiseDietMsgException("DIET call failure");
	}

	if(diet_string_get(diet_parameter(getCompletedJobsOutputProfile,3), &listJobResultInString, NULL)){
		msgErrorDiet += " by receiving User serialized  message";
		raiseDietMsgException(msgErrorDiet);
	}

	if(diet_string_get(diet_parameter(getCompletedJobsOutputProfile,4), &errorInfo, NULL)){
		msgErrorDiet += " by receiving errorInfo message";
		raiseDietMsgException(msgErrorDiet);
	}

	raiseExceptionIfNotEmptyMsg(errorInfo); /*To raise a vishnu exception if the receiving message is not empty*/
	parseEmfObject(std::string(listJobResultInString), listJobResults_ptr); /*To build the listJobResults_ptr */
	try {
		IDContainer = (getCompletedJobsOutputProfile->parameters[5]).desc.id;
		dagda_get_container(IDContainer);
		dagda_get_container_elements(IDContainer, &content);
		if (content.size != 0) {
			/* Get first the description file */
			size_t index = 0 ;
			char* fName = NULL;
			dagda_get_file(content.elt_ids[index++], &fName) ;
			std::istringstream fdescStream (vishnu::get_file_content(fName, false)) ;
			size_t jnum = 0 ;
			std::string line ;
			ListStrings lineVec ;
			while( getline(fdescStream, line) ) {
				boost::trim(line) ; boost::split(lineVec, line, boost::is_any_of(" ")) ;

				moutDir = (bfs::path(bfs::current_path().string())).string() + "/DOWNLOAD_" + lineVec[0] ;
				vishnu::createOutputDir(moutDir);
				listJobResults_ptr->getResults().get(jnum++)->setOutputDir(moutDir);

				std::cout << "LINE " << line << std::endl;
				int nbFiles = lineVec.size() ;
				for(int i = 1; i < nbFiles; i++) {
					if( index >= content.size ) {
						throw UserException(ERRCODE_INVALID_PARAM,
								"DAGDA Exception: uncompleted list of transfered files");
					}
					dagda_get_file(content.elt_ids[index++], &fName);
					vishnu::boostMoveFile(std::string(fName), moutDir, boost::trim_copy(lineVec[i]));
					free(fName) ;
				}
			}
			for(unsigned int i = 0; i < content.size; i++) {
				dagda_delete_data(content.elt_ids[i]);
			}
		}
	} catch (CORBA::Exception & ex) {//To catch CORBA exception sent by DAGDA
		for(unsigned int i = 0; i < content.size; i++) { dagda_delete_data(content.elt_ids[i]); }
		dagda_delete_data(IDContainer);
		diet_profile_free(getCompletedJobsOutputProfile);
		throw UserException(ERRCODE_INVALID_PARAM, "CORBA Exception: "+std::string(ex._name())+
				" ("+std::string(IDContainer) + ")");
	}

	//To clean the container Id
	dagda_delete_data(IDContainer);
	diet_profile_free(getCompletedJobsOutputProfile);
	return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
