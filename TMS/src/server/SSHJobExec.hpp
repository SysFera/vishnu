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

    SSHJobExec(char* script_path, 
               const std::string& jobSerialized, 
               const std::string& submitOptionsSerialized,
               BatchType batchType);
     
    std::string getJobSerialized();

    std::string getErrorInfo(); //Temporaire

    int sshexec(const std::string& cmd);

    ~SSHJobExec();

  private:
    void createTmpFile(char* outputFilePath, const std::string& file_content);
    void createTmpFile(char* fileName);

    char*  mscript_path;
    std::string mjobSerialized;
    std::string msubmitOptionsSerialized;
    BatchType mbatchType;
    std::string merrorInfo;
};

#endif
