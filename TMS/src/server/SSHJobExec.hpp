/**
 * \file SSHJobExec.hpp
 * \brief This file presents the implementation of the SSHJobExec.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 
*/

#ifndef _SSHJobExec_H_
#define _SSHJobExec_H_

#include <string>
#include "utilVishnu.hpp"

/**
 * \class SSHJobExec 
 * \brief SSHJobExec class implementation
 */
class SSHJobExec {

  public:

    /**
     * \brief Constructor
     */
    SSHJobExec();

     /**
     * \brief Constructor 
     * \param scriptPath the path to the script containing the job characteristique
     * \param jobSerialized the job serialized
     * \param submitOptionsSerialized the job options serialized
     * \param user the user login
     * \param hostname the hostname of the machine 
     * \param key the private key
     * \param batchType the type of the batch scheduler
     */
    SSHJobExec(char* script_path, 
               const std::string& jobSerialized, 
               const std::string& submitOptionsSerialized,
               const std::string& user, 
               const std::string& hostname,
               const std::string& key,
               BatchType batchType);
    
     /**
     * \brief Function to execute command by using ssh 
     * \param slaveDirectory the path to the command executable
     * \param cmd the arguments of the command 
     * \return raises an exception on error
     */
    int 
    sshexec(const std::string& slaveDirectory,
                const std::string& cmd);

    /**
     * \brief Function to copy files from remote machine
     * \param outputPath the output path to get
     * \param errorPath the error path to get
     * \param copyOfOutputPath the copy of the outputPath
     * \param copyOfErrorPath the copy of errorPath 
     * \return raises an exception on error
     */ 
    int 
    copyFiles(const std::string& outputPath, 
                  const std::string& errorPath, 
                  const char* copyOfOutputPath, 
                  const char* copyOfErrorPath);

    std::string 
    getJobSerialized();

    std::string 
    getErrorInfo();
    
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
