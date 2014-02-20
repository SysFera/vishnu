/**
 * \file QueryServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include "DbFactory.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"
#include "constants.hpp"

/**
 * \class QueryServer
 * \brief QueryServer class implementation
 */
template <class QueryParameters, class ListObject>
class QueryServer
{
public:

  /**
    * \brief Constructor, raises an exception on error
    */
  explicit
  QueryServer(void)
  {
    mlistObject = NULL;
    DbFactory factory;
    mdatabaseInstance = factory.getDatabaseInstance();
  }

  /**
   * \brief Function to list query information
   * \return The pointer to the ListOject containing list information
   * \return raises an exception on error
   */
  virtual ListObject* list(QueryParameters* parameters) = 0;

  /**
   * \brief Function to get the name of the query command line
   * \return The the name of the query command line
   */
  virtual std::string getCommandName() = 0;

  /**
   * \brief Destructor, raises an exception on error
   */
  virtual ~QueryServer()
  {
  }

protected:
  /**
   * \brief Function to add sql resquest "and condition" to a given request
   * \param name The column name of the data base table
   * \param value The value to search in the given column
   * \param request The request
   */
  void addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
    request.append(" and "+name+"=");
    request.append("'"+mdatabaseInstance->escapeData(value)+"'");
  }

  /**
   * \brief Function to add sql resquest "and condition" which contain an integer value to a given request
   * \param name The column name of the data base table
   * \param value The integer value to search in the given column
   * \param request the request
   */
  template <class T>
  void addIntegerOptionRequest(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" and "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  /**
   * \brief Function to add sql resquest "and condition" which contain an integer value to a given request
   * \param name The column name of the data base table
   * \param value The integer value to search in the given column
   * \param request the request
   * \param comp The where statement
   */
  template <class T>
  void addTimeRequest(const std::string& name, T& value, std::string& request, std::string comp) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" and "+name+ " "+comp+" ");
    request.append("'"+osValue.str()+"'");
  }

  /**
   * \brief Function to add sql resquest "where condition" to a given request
   * \param name The column name of the data base table
   * \param value The value to search in the given column
   * \param request The request
   */
  void addCondition(const std::string& name, const std::string& value, std::string& request) {
    request.append(" where "+name+"=");
    request.append("'"+mdatabaseInstance->escapeData(value)+"'");
  }

  /**
   * \brief Function to add sql resquest "where condition" which contain an integer value to a given request
   * \param name The column name of the data base table
   * \param value The integer value to search in the given column
   * \param request the request
   */
  template <class T>
  void addIntegerCondition(const std::string& name, T& value, std::string& request) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" where "+name+"=");
    request.append("'"+osValue.str()+"'");
  }

  /**
   * \brief Function to convert a given date into correspondant long value
   * \param date The date to convert
   * \return The converted value
   */
  long long convertToTimeType(std::string date) {
    if(date.size()==0 ||
       // For mysql, the empty date is 0000-00-00, not empty, need this test to avoid problem in ptime
       date.find("0000-00-00")!=std::string::npos) {
      return 0;
    }

    boost::posix_time::ptime pt(boost::posix_time::time_from_string(date));
    boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
    boost::posix_time::time_duration::sec_type time = (pt - epoch).total_seconds();

    return (long long) time_t(time);

  }

  /**
   * \brief Function to check if a given user identifier exists
   * \param userId the user identifier
   * \return raises an exception on error
   */
  void checkUserId(std::string userId) {
    std::string sqlUserRequest = (boost::format("SELECT userid"
                                                " FROM users"
                                                " WHERE userid='%1%'"
                                                " AND status<>%2%")%mdatabaseInstance->escapeData(userId) %vishnu::STATUS_DELETED).str();
    boost::scoped_ptr<DatabaseResult> user(mdatabaseInstance->getResult(sqlUserRequest.c_str()));
    if(user->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
    }
  }

  /**
   * \brief Function to check if a given machine identifier exists
   * \param machineId the machine identifier
   */
  void checkMachineId(std::string machineId) {
    std::string sqlMachineRequest = (boost::format("SELECT machineid"
                                                   " FROM machine"
                                                   " WHERE machineid='%1%'"
                                                   " AND status<>%2%")%mdatabaseInstance->escapeData(machineId) %vishnu::STATUS_DELETED).str();
    boost::scoped_ptr<DatabaseResult> machine(mdatabaseInstance->getResult(sqlMachineRequest.c_str()));
    if(machine->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
  }

  /**
   * \brief Function to check if a given machine client identifier exists
   * \param clmachineId the machine client identifier
   */
  void checkClientMachineName(std::string clmachineId) {
    std::string sqlclMachineRequest = (boost::format("SELECT name"
                                                     " FROM clmachine"
                                                     " WHERE name='%1%'")%mdatabaseInstance->escapeData(clmachineId)).str();
    boost::scoped_ptr<DatabaseResult> clmachine(mdatabaseInstance->getResult(sqlclMachineRequest.c_str()));
    if(clmachine->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
  }

  /**
   * \brief Function to check if a given option name exists
   * \param name The name of the option
   * \return raises an exception on error
   */
  void checkOptionName(std::string name) {
    std::string sqlNameRequest = (boost::format("SELECT description"
                                                " FROM optionu"
                                                " WHERE description='%1%'")%mdatabaseInstance->escapeData(name)).str();
    boost::scoped_ptr<DatabaseResult> nameResults(mdatabaseInstance->getResult(sqlNameRequest.c_str()));
    if(nameResults->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION);
    }
  }

  /**
   * \brief Function to check if a given session identifier exists
   * \param sessionId the session identifier
   */
  void checkSessionId(std::string sessionId) {
    std::string sqlSessionRequest = (boost::format("SELECT vsessionid"
                                                   " FROM vsession"
                                                   " WHERE vsessionid='%1%'"
                                                   " AND state<>%2%")%mdatabaseInstance->escapeData(sessionId) %vishnu::STATUS_DELETED).str();
    boost::scoped_ptr<DatabaseResult> session(mdatabaseInstance->getResult(sqlSessionRequest.c_str()));
    if(session->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_SESSION_ID);
    }
  }

  /**
   * \brief Function to check a status value
   * \param status The status to check
   */
  void checkStatus(const int& status) {
    if((status < -1) || (status > 1)) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION, "Invalid value: its value must be 0 or 1");
    }
  }

  /**
   * \brief Function to check a close policy value
   * \param closePolicy The closePolicy value to check
   */
  void checkClosePolicy(const int& closePolicy) {
    if((closePolicy < 0) || (closePolicy > 2)) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_CLOSURE_MODE, "Invalid ClosePolicy value: its value must be 0, 1 or 2");
    }
  }


  /**
   * \brief Function to check if a given job identifier exists
   * \param jobId the job identifier
   */
  void
  checkJobId(std::string jobId) {
    std::string sqlJobRequest = (boost::format("SELECT numjobid"
                                               " FROM job"
                                               " WHERE jobId='%1%'")%mdatabaseInstance->escapeData(jobId)).str();
    boost::scoped_ptr<DatabaseResult> result (mdatabaseInstance->getResult(sqlJobRequest.c_str()));
    if(result->getNbTuples() == 0) {
      throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
    }
  }

  /**
   * \brief Function to check if a given authSystem identifier exists
   * \param authSystemId the authSystem identifier
   */
  void
  checkAuthSystemId(std::string authSystemId) {
    std::string sqlJobRequest = (boost::format("SELECT authsystemid"
                                               " FROM authsystem"
                                               " WHERE authsystemid='%1%'"
                                               " AND status<>%2%")%mdatabaseInstance->escapeData(authSystemId) %vishnu::STATUS_DELETED).str();
    boost::scoped_ptr<DatabaseResult> result (mdatabaseInstance->getResult(sqlJobRequest.c_str()));
    if(result->getNbTuples() == 0) {
      throw TMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM);
    }
  }

protected:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The Object containing query information
  */
  ListObject* mlistObject;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseInstance;

};

#endif
