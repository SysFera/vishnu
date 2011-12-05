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


void Env::replaceAllOccurences(std::string& scriptContent,
    const std::string& oldValue,
    const std::string& newValue) {

  size_t pos = scriptContent.find(oldValue);
  while(pos!=std::string::npos) {
    scriptContent.replace(pos, oldValue.size(), newValue, 0, newValue.size());
    pos = scriptContent.find(oldValue, pos+newValue.size());
  }

}


void Env::getTorqueNumNodesAndCpuPerNode(std::string& scriptContent,
                                         std::string& numNodes, 
                                         std::string& cpuPerNode) {

  numNodes="";
  cpuPerNode="";
  
  std::string line;
  std::ostringstream osNumNodes;
  std::ostringstream osCpuPerNode;
  bool numNodesFound = false;
  bool cpuPerNodeFound = false;
  size_t pos;
  std::istringstream is(scriptContent);
  
  while(!is.eof()) {

    getline(is, line);
    pos = line.find("#PBS");
    if(pos!=std::string::npos) {
      if(line.find("-l", pos)!=std::string::npos){
        size_t pos2 = line.find("nodes=", pos);
        if(pos2!=std::string::npos) {
          numNodesFound = true;
          std::string nbNodes = line.substr(pos2+std::string("nodes=").size());
          std::istringstream is(nbNodes);
          int value;
          is >> value;
          osNumNodes.str("");
          osNumNodes << value;

          if(line.find(":", pos)!=std::string::npos){
            pos2 = line.find("ppn=", pos);
            if(pos2!=std::string::npos) {
              cpuPerNodeFound = true;
              std::string nbNodes = line.substr(pos2+std::string("ppn=").size());
              std::istringstream is(nbNodes);
              osCpuPerNode.str("");
              is >> value;
              osCpuPerNode << value;
            }
          } else {
            cpuPerNodeFound =  false;
          }
        }
      }
    }
  }

  if(numNodesFound) { 
    numNodes = osNumNodes.str();
  }

  if(cpuPerNodeFound) { 
    cpuPerNode =  osCpuPerNode.str();
  }
}

/*
*/
void Env::replaceEnvVariables(std::string& scriptContent) {

  std::string numNodes;
  std::string cpuPerNode;

  switch(mbatchType) {

    case TORQUE:

      //To replace VISHNU_JOB_ID
      replaceAllOccurences(scriptContent, "VISHNU_JOB_ID", "PBS_JOBID");
      //To replace VISHNU_JOB_NAME
      replaceAllOccurences(scriptContent, "VISHNU_JOB_NAME", "PBS_JOBNAME");
      //To replace VISHNU_JOB_NODEFILE
      replaceAllOccurences(scriptContent, "VISHNU_JOB_NODEFILE", "PBS_NODEFILE");
      //To replace VISHNU_JOB_NODELIST
      replaceAllOccurences(scriptContent, "$VISHNU_JOB_NODELIST", "\"$(cat $PBS_NODEFILE)\"");
      replaceAllOccurences(scriptContent, "${VISHNU_JOB_NODELIST}", "\"$(cat $PBS_NODEFILE)\"");

      getTorqueNumNodesAndCpuPerNode(scriptContent, numNodes, cpuPerNode);
      replaceAllOccurences(scriptContent, "$VISHNU_JOB_NUM_NODES", numNodes);
      replaceAllOccurences(scriptContent, "${VISHNU_JOB_NUM_NODES}", numNodes);

      replaceAllOccurences(scriptContent, "$VISHNU_JOB_CPUS_PER_NODE", cpuPerNode);
      replaceAllOccurences(scriptContent, "${VISHNU_JOB_CPUS_PER_NODE}", cpuPerNode);

      break;

    case LOADLEVELER:
      break;
    case SLURM:
      replaceAllOccurences(scriptContent, "VISHNU_JOB_ID", "SLURM_JOB_ID");
      replaceAllOccurences(scriptContent, "VISHNU_JOB_NAME", "SLURM_JOB_NAME");
      replaceAllOccurences(scriptContent, "VISHNU_JOB_NODELIST", "SLURM_JOB_NODELIST");
      replaceAllOccurences(scriptContent, "VISHNU_JOB_NUM_NODES", "SLURM_JOB_NUM_NODES");
      replaceAllOccurences(scriptContent, "VISHNU_JOB_CPUS_PER_NODE", "SLURM_JOB_CPUS_PER_NODE");
      break;
    default:
      break;

  }
}
