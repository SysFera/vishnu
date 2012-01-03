/**
 * \file Env.cpp
 * \brief This file contains the VISHNU Env class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date December 2011
 */

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
 * \brief Function to get number of nodes for torque 
 * \param scriptContent The string content to modify
 */
void Env::getTorqueNumNodes(std::string& scriptContent) {

  size_t pos = scriptContent.find("VISHNU_BATCHJOB_NUM_NODES");
  if(pos!=std::string::npos) {
    pos = scriptContent.rfind("\n", pos-1);
    std::string tmp = "NBNODES=\"$(cat  $PBS_NODEFILE | wc -l)\" \n";
    scriptContent.insert(pos+1, tmp);
    replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NUM_NODES", "$NBNODES");
    replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NUM_NODES}", "$NBNODES");
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
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_ID", "PBS_JOBID");
      //To replace VISHNU_BATCHJOB_NAME
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NAME", "PBS_JOBNAME");
      //To replace VISHNU_BATCHJOB_NODEFILE
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NODEFILE", "PBS_NODEFILE");
      //To replace VISHNU_BATCHJOB_NODEFILE
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NODEFILE", "PBS_NODEFILE");
      //To replace VISHNU_BATCHJOB_NUM_NODES
      getTorqueNumNodes(scriptContent);

      break;

    case LOADLEVELER:
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_ID", "LOADL_STEP_ID"); 
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NAME", "LOADL_JOB_NAME"); 
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NODEFILE", "LOADL_HOSTFILE"); 
      break;
    case SLURM:
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_ID", "SLURM_JOB_ID");
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NAME", "SLURM_JOB_NAME");
      replaceAllOccurences(scriptContent, "VISHNU_BATCHJOB_NUM_NODES", "SLURM_JOB_NUM_NODES");
      //replaceAllOccurences(scriptContent, "VISHNU_JOB_CPUS_PER_NODE", "SLURM_JOB_CPUS_PER_NODE");
      
      pos = scriptContent.find("VISHNU_BATCHJOB_NODEFILE");
      if(pos!=std::string::npos) {
        std::string fileName = std::string(getenv("HOME"))+"/NODELIST_XXXXXX";
        vishnu::createTmpFile(const_cast<char*>(fileName.c_str()));
        pos = scriptContent.rfind("\n", pos-1);
        scriptContent.insert(pos+1, "echo $SLURM_JOB_NODELIST > "+fileName+"\n");
        std::string tmp = "echo $SLURM_JOB_NODELIST > "+fileName+"\n";
        scriptContent.insert(pos+1+tmp.size(), "sed -i 's/,/\\n/g' "+fileName+"\n");
        replaceAllOccurences(scriptContent, "$VISHNU_BATCHJOB_NODEFILE", fileName);
        replaceAllOccurences(scriptContent, "${VISHNU_BATCHJOB_NODEFILE}", fileName);

      }

     break;
    default:
      break;

  }
}
