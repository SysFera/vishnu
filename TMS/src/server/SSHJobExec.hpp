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
     
    std::string getJobSerialized();

    std::string getErrorInfo(); //Temporaire

    int sshexec(const std::string& cmd);
    void createTmpFile(char* outputFilePath, const std::string& file_content);
    void createTmpFile(char* fileName);
    int deleteFile(const char* fileName);

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
