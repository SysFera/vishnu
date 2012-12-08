/**
 * \file Env.cpp
 * \brief This file contains the VISHNU Env class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date December 2011
 */

#include <boost/algorithm/string.hpp>
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
void
Env::replaceEnvVariables(std::string& scriptContent) {
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

<<<<<<< HEAD
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
    case PBSPRO:
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
      		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$VISHNU_BATCHJOB_NODEFILE");
     		pos = scriptContent.find("$VISHNU_BATCHJOB_NODEFILE");
     		if(pos!=std::string::npos) {
        		std::string fileName = "/tmp/SGE_NODELIST_XXXXXX";
        		vishnu::createTmpFile(const_cast<char*>(fileName.c_str()));
        		pos = scriptContent.rfind("\n", pos-1);
        		scriptContent.insert(pos+1, "echo $HOSTNAME > "+fileName+"\n");
        		std::string tmp = "echo $HOSTNAME > "+fileName+"\n";
        		scriptContent.insert(pos+1+tmp.size(), "sed -i 's/ /\\n/g' "+fileName+"\n");
        		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", fileName);
        		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", fileName);
     		}
     		//To replace VISHNU_BATCHJOB_NUM_NODES
      		replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$NHOSTS");
     		replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$NHOSTS");
		break;
	default:
		break;

	}
=======
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

    if (pos!=std::string::npos) {
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
    if (pos!=std::string::npos) {
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
    break;

  case SGE:
    //To replace VISHNU_BATCHJOB_ID
    replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_ID", "$JOB_ID");
    replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_ID}", "$JOB_ID");
    //To replace VISHNU_BATCHJOB_NAME
    replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NAME", "$JOB_NAME");
    replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NAME}", "$JOB_NAME");
    //To replace VISHNU_BATCHJOB_NODEFILE
    replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", "$VISHNU_BATCHJOB_NODEFILE");
    pos = scriptContent.find("$VISHNU_BATCHJOB_NODEFILE");
    if (pos!=std::string::npos) {
      std::string fileName = "/tmp/SGE_NODELIST_XXXXXX";
      vishnu::createTmpFile(const_cast<char*>(fileName.c_str()));
      pos = scriptContent.rfind("\n", pos-1);
      scriptContent.insert(pos+1, "echo $HOSTNAME > "+fileName+"\n");
      std::string tmp = "echo $HOSTNAME > "+fileName+"\n";
      scriptContent.insert(pos+1+tmp.size(), "sed -i 's/ /\\n/g' "+fileName+"\n");
      replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", fileName);
      replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", fileName);
    }
    //To replace VISHNU_BATCHJOB_NUM_NODES
    replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$NHOSTS");
    replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$NHOSTS");
    break;

  default:
    break;
  }
>>>>>>> Added missing break in switch/case
}

/**
 * \brief function to set parameters appearing in a script
 * \param scriptContent The string to modify
 * \param \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
 */
void
Env::setParams(std::string& scriptContent,
               const std::string & params) {
  std::string paramName;
  std::string paramValue;
  size_t pos;
  ListStrings paramsVec;

  std::string& refParams = const_cast<std::string&>(params);
  boost::trim(refParams);
  boost::split(paramsVec, refParams, boost::is_any_of(" "));
  for (ListStrings::iterator it = paramsVec.begin(); it != paramsVec.end(); ++it) {
    pos = it->find("=");
    if (pos != std::string::npos) {
      paramName = it->substr(0, pos);
      paramValue = it->substr(pos+1, std::string::npos);
      replaceAllOccurences(scriptContent, "$" + paramName, paramValue);
      replaceAllOccurences(scriptContent, "${" + paramName + "}", paramValue);
    }
  }
}

/**
 * \brief Function to set environment variables accordinf to parameters
 * \param params a list of parameters in the form of PARAM1=value1  PARAM2=value2 ...
 */
void
Env::setParamsEnvVars(const std::string& params) {
  std::string param;
  size_t pos, pos1, pos2;

  pos1 = 0;
  while (pos2 = params.find(" ", pos1), pos2 != std::string::npos) {
    param = params.substr(pos1, pos2);
    pos = param.find("=");
    if (pos != std::string::npos) {
      setenv(param.substr(0, pos).c_str(),
             param.substr(pos + 1, std::string::npos).c_str(), 1);
    }
    pos1 = pos2 + 1;
  }
  param = params.substr(pos1, std::string::npos); //last token
  pos = param.find("=");
  if (pos != std::string::npos ) {
    setenv(param.substr(0, pos).c_str(),
           param.substr(pos+1, std::string::npos).c_str(), 1);
  }
}
