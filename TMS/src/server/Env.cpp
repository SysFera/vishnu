/**
 * \file Env.cpp
 * \brief This file contains the VISHNU Env class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date December 2011
 */
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include "Env.hpp"

/**
 * \param batchType The batch scheduler type
 * \brief Constructor
 */
Env::Env(const BatchType& batchType): mbatchType(batchType) {
}

/**
 * \brief Function to replace all occurences in a string
 * \param scriptContent The string to modify
 * \param oldValue The value to replace
 * \param newValue The new value
 */

void Env::replaceAllOccurences(std::string& scriptContent,
		const std::string& oldValue,
		const std::string& newValue) {

	size_t pos = scriptContent.find(oldValue);
	while(pos!=std::string::npos) {
		scriptContent.replace(pos, oldValue.size(), newValue, 0, newValue.size());
		pos = scriptContent.find(oldValue, pos+newValue.size());
	}

}

/**
 * \brief Function to replace some environment variables in a string
 * \param scriptContent The string content to modify
 */
void Env::replaceEnvVariables(std::string& scriptContent) {

	std::string numNodes;
	std::string cpuPerNode;
	size_t pos;
	switch(mbatchType) {

	case TORQUE:
		//To replace VISHNU_BATCHJOB_ID
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$PBS_JOBID");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$PBS_JOBID");
		//To replace VISHNU_BATCHJOB_NAME
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$PBS_JOBNAME");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$PBS_JOBNAME");
		//To replace VISHNU_BATCHJOB_NODEFILE
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", "$PBS_NODEFILE");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$PBS_NODEFILE");
		//To replace VISHNU_BATCHJOB_NUM_NODES
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$(cat  $PBS_NODEFILE | sort | uniq | wc -l)");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$(cat  $PBS_NODEFILE | sort | uniq | wc -l)");
		break;
	case LOADLEVELER:
		//To replace VISHNU_BATCHJOB_ID
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$LOADL_STEP_ID");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$LOADL_STEP_ID");
		//To replace VISHNU_BATCHJOB_NAME
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$LOADL_JOB_NAME");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$LOADL_JOB_NAME");
		//To replace VISHNU_BATCHJOB_NODEFILE
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", "$LOADL_HOSTFILE");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$LOADL_HOSTFILE");
		//To replace VISHNU_BATCHJOB_NUM_NODES
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$(cat  $LOADL_HOSTFILE | sort | uniq | wc -l)");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$(cat  $LOADL_HOSTFILE | sort | uniq | wc -l)");
		break;
	case SLURM:
		//To replace VISHNU_BATCHJOB_ID
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$SLURM_JOB_ID");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$SLURM_JOB_ID");
		//To replace VISHNU_BATCHJOB_NAME
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$SLURM_JOB_NAME");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$SLURM_JOB_NAME");
		//To replace SLURM_JOB_NUM_NODES
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$SLURM_JOB_NUM_NODES");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$SLURM_JOB_NUM_NODES");

		//To replace VISHNU_BATCHJOB_NODEFILE
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$VISHNU_BATCHJOB_NODEFILE");
		pos = scriptContent.find("$VISHNU_BATCHJOB_NODEFILE");
		if(pos!=std::string::npos) {
			std::string fileName = "/tmp/NODELIST_XXXXXX";
			vishnu::createTmpFile(const_cast<char*>(fileName.c_str()));
			pos = scriptContent.rfind("\n", pos-1);
			scriptContent.insert(pos+1, "echo $SLURM_JOB_NODELIST > "+fileName+"\n");
			std::string tmp = "echo $SLURM_JOB_NODELIST > "+fileName+"\n";
			scriptContent.insert(pos+1+tmp.size(), "sed -i 's/,/\\n/g' "+fileName+"\n");
			replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", fileName);
			replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", fileName);
			scriptContent.insert(scriptContent.size()-1, "\n rm "+fileName+"\n");
		}

		break;
	case LSF:
		//To replace VISHNU_BATCHJOB_ID
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$LSB_JOBID");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$LSB_JOBID");
		//To replace VISHNU_BATCHJOB_NAME
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$LSB_JOBNAME");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$LSB_JOBNAME");
		//To replace VISHNU_BATCHJOB_NODEFILE
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$VISHNU_BATCHJOB_NODEFILE");
		pos = scriptContent.find("$VISHNU_BATCHJOB_NODEFILE");
		if(pos!=std::string::npos) {
			std::string fileName = "/tmp/LSF_NODELIST_XXXXXX";
			vishnu::createTmpFile(const_cast<char*>(fileName.c_str()));
			pos = scriptContent.rfind("\n", pos-1);
			scriptContent.insert(pos+1, "echo $LSB_HOSTS > "+fileName+"\n");
			std::string tmp = "echo $LSB_HOSTS > "+fileName+"\n";
			scriptContent.insert(pos+1+tmp.size(), "sed -i 's/ /\\n/g' "+fileName+"\n");
			replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", fileName);
			replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", fileName);
			//To replace VISHNU_BATCHJOB_NUM_NODES
			replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$(cat "+fileName+" | sort | uniq | wc -l)");
			replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$(cat "+fileName+" | sort | uniq | wc -l)");
			scriptContent.insert(scriptContent.size()-1, "\n rm "+fileName+"\n");
		}
	case SGE:
		//To replace VISHNU_BATCHJOB_ID
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$JOB_ID");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$JOB_ID");
		//To replace VISHNU_BATCHJOB_NAME
		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$JOB_NAME");
		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$JOB_NAME");
		//To replace VISHNU_BATCHJOB_NODEFILE
		/*replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", "$PBS_NODEFILE");
      //replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$PBS_NODEFILE");
      //To replace VISHNU_BATCHJOB_NUM_NODES
      replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$(cat  $PBS_NODEFILE | sort | uniq | wc -l)");
      replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$(cat  $PBS_NODEFILE | sort | uniq | wc -l)");*/
		break;
	default:
		break;

	}
}

/**
 * \brief Function to set textual parameter appearing in a string
 * \param scriptContent The string to modify
 * \param textParams list of textual parameters in the form of PARAM1="value1" PARAM2="value2" ...
 * \param fileParams list of file parameters in the form of FILE1="/path/to/file1"  FILE2="/path/to/file2" ...
 */
void Env::setParameters(std::string& scriptContent, const std::string& textParams, const std::string& fileParams){

	ListStrings paramsVec ;
	if( textParams.size() != 0 ) {
		/* Set textual parameters if there exist */
		boost::split(paramsVec, textParams, boost::is_any_of(" ")) ;
		for(ListStrings::const_iterator it = paramsVec.begin(); it != paramsVec.end(); it++){
			ListStrings attrs ;
			boost::split(attrs, *it, boost::is_any_of("=")) ;
			replaceAllOccurences(scriptContent, "$" + attrs[0], attrs[1]) ;
			replaceAllOccurences(scriptContent, "${" + attrs[0] + "}", attrs[1]) ;
		}
	}

	/* Set file parameters if there exist*/
	if( fileParams.size() != 0) {
		boost::split(paramsVec, fileParams, boost::is_any_of(" ")) ;
		for(ListStrings::const_iterator it = paramsVec.begin(); it != paramsVec.end(); it++){
			ListStrings attrs ;
			boost::split(attrs, *it, boost::is_any_of("=")) ;
			replaceAllOccurences(scriptContent, "$" + attrs[0], "/tmp/" + attrs[0]) ;
			replaceAllOccurences(scriptContent, "${" + attrs[0] + "}", "/tmp/" + attrs[0]) ;
		}
	}
}
