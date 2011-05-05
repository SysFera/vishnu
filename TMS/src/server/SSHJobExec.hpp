/**
 * \file TMSServer.hpp
 * \brief This file presents the implementation of the SSHJobExec.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 
*/

#ifndef _SSHJobExec_H_
#define _SSHJobExec_H_

#include <string>
#include "utilVishnu.hpp"

class SSHJobExec {

  public:

    SSHJobExec();

    SSHJobExec(char* script_path, 
               const std::string& jobSerialized, 
               const std::string& submitOptionsSerialized,
               const std::string& user, 
               const std::string& hostname,
               const std::string& key,
               BatchType batchType);
     
    int sshexec(const std::string& slaveDirectory,
                const std::string& cmd);

    int copyFiles(const std::string& outputPath, 
                  const std::string& errorPath, 
                  const char* copyOfOutputPath, 
                  const char* copyOfErrorPath);

    std::string getJobSerialized();

    std::string getErrorInfo();
    
    ~SSHJobExec();

  private:
    std::string convertBatchTypeToString(BatchType batchType);
    char*  mscript_path;
    std::string mjobSerialized;
    std::string msubmitOptionsSerialized;
    BatchType mbatchType;
    std::string merrorInfo;
    std::string muser;
    std::string msshKey;
    std::string mhostname;
};

#endif
