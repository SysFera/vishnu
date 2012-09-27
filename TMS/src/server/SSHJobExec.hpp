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
     * \param user the user login
     * \param hostname the hostname of the machine
     * \param batchType the type of the batch scheduler
     * \param jobSerialized the job serialized
     * \param submitOptionsSerialized the job options serialized
     */
    SSHJobExec(const std::string& user,
               const std::string& hostname,
               const BatchType& batchType = UNDEFINED,
               const std::string& jobSerialized = "",
               const std::string& submitOptionsSerialized="");

     /**
     * \brief Function to execute command by using ssh
     * \param slaveDirectory the path to the command executable
     * \param serviceName the name of the service to execute
     * \param script_path the path to script to submit
     * \return raises an exception on error
     */
    void
    sshexec(const std::string& slaveDirectory,
            const std::string& serviceName,
            const std::string& script_path="");

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

    /**
     * \brief Function to copy files from remote machine
     * \param path the path of the file
     * \return raises an exception on error
     */
    int
    copyFile(const std::string& path, const std::string& dest) ;

    /**
     * \brief Function to execute a command via ssh
     * \param cmd the command to execute
     */
    int
    execCmd(const std::string& cmd);

    /**
     * \brief Function to return the job serialized content
     * \return  job serialized content
     */
    std::string
    getJobSerialized();

     /**
     * \brief Function to return the error message of a service
     * \return error message information
     */
    std::string
    getErrorInfo();

    /**
     * \brief Destructor
     */
    ~SSHJobExec();

  private:

    /**
     * \brief Function to convert the batch type to string
     * \param batchType the batch type to convert
     * \return the converted batch type
     */
    std::string convertBatchTypeToString(BatchType batchType);

    /**
     * \brief Function to check the parameters before launching ssh
     * \return raises an exception on error
     */
    void checkSshParams();

    /**
     * \brief The job serialized
     */
    std::string mjobSerialized;

    /**
     * \brief The job options serialized
     */
    std::string msubmitOptionsSerialized;

    /**
     * \brief The type of the batch scheduler
     */
    BatchType mbatchType;

    /**
     * \brief The message erroro occured during execution of a service
     */
    std::string merrorInfo;

    /**
     * \brief The user login
     */
    std::string muser;

    /**
     * \brief The hostname of the machine
     */
    std::string mhostname;

};

#endif
