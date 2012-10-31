/**
  * \file JobServer.hpp
  * \brief This file contains the VISHNU JobServer class.
  * \author Daouda Traore (daouda.traore@sysfera.com)
  * \date April 2011
  */

#ifndef _JOB_SERVER_H
#define _JOB_SERVER_H

#include "TMS_Data.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
/**
 * \class JobServer
 * \brief JobServer class implementation
 */
class JobServer
{
 
public:
	/**
  * \param sessionServer The object which encapsulates the session information
  * \param machineId The machine identifier 
  * \param job The job data structure
  * \param batchType The batch scheduler type
  * \brief Constructor
  */
  explicit JobServer(const SessionServer& sessionServer,
                     const std::string& machineId,
                     const TMS_Data::Job& job,
                     const BatchType& batchType);
  /**
   * \brief Function to submit job
   * \param scriptContent the content of the script
   * \param options the options to submit job
   * \param vishnuId The VISHNU identifier
   * \param slaveDirectory the path to the TMS slave executable
   * \return raises an exception on error
   */
	int submitJob(const std::string& scriptContent, 
                const TMS_Data::SubmitOptions& options,
                const int& vishnuId,
                const std::string& slaveDirectory,
                const std::vector<std::string>& defaultBatchOption);

  /**
   * \brief Function to cancel job
   * \param slaveDirectory the path to the TMS slave executable
   * \return raises an exception on error
   */
	int cancelJob(const std::string& slaveDirectory);

  /**
   * \brief Function to get job information
   * \return The job data structure
   */
  TMS_Data::Job getJobInfo(); 

  /**
   * \brief Function to get job data 
   * \return The job data structure
   */
  TMS_Data::Job getData();

  /**
   * \brief Destructor
   */
  ~JobServer(); 
  

  private:

  /**
   * \brief Function to scan VISHNU error message 
   * \param errorInfo the error information to scan
   * \param code The code The code of the error
   * \param message The message associeted to the error code
   * \return raises an exception on erroor
   */
  void scanErrorMessage(const std::string& errorInfo, int& code, std::string& message);
  
  /**
   * \brief Function to convert a given date into correspondant long value
   * \fn long long convertToTimeType(std::string date)
   * \param date The date to convert
   * \return The converted value
   */
  long long convertToTimeType(std::string date);
  
  /**
   * \brief Function to insert option line into string
   * \param optionLineToInsert the option to insert
   * \param content The buffer containing the inserted option
   * \return raises an exception on error
   */
  void
  insertOptionLine(std::string& optionLineToInsert, std::string& content, std::string& key);

  /**
   * \brief job data structure 
   */ 
  TMS_Data::Job mjob;

  /**
   * \brief The object which encapsulates the session information 
   */
  SessionServer msessionServer;

  /**
   * \brief The machine identifier 
   */   
  std::string mmachineId;
  
  /**
   * \brief The type of the batch scheduler 
   */
  BatchType mbatchType;
  
  /**
   * \brief An instance of vishnu database
   */
  Database *mdatabaseVishnu;
};

#endif
