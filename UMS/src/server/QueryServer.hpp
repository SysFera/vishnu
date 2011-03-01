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
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"

/**
 * \class QueryServer 
 * \brief QueryServer class implementation 
 */
template <class QueryParameters, class ListObject>
class QueryServer
{
public:

   /**
   * \fn QueryServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  explicit
  QueryServer(const SessionServer session):msessionServer(session)
  {
    mlistObject = NULL;
    DbFactory factory;
    mdatabaseVishnu = factory.getDatabaseInstance();
  }

  /**
   * \fn QueryServer(const QueryParameters* params,
   *                 const SessionServer& session)
   * \param params The object which encapsulates the information of query options 
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  QueryServer(QueryParameters *params, const SessionServer& session):
    mparameters(params), msessionServer(session)
  {

    mlistObject = NULL;
    DbFactory factory;
    mdatabaseVishnu = factory.getDatabaseInstance();
  };

  /**
   * \brief Function to list query information 
   * \fn virtual ListObject* list() = 0
   * \return The pointer to the ListOject containing list information
   * \return raises an exception on error
   */
  virtual ListObject* list() = 0;

  /**
   * \brief Function to get the name of the query command line 
   * \fn  virtual std::string getCommandName() = 0 
   * \return The the name of the query command line 
   */
  virtual std::string getCommandName() = 0;

  /**
   * \brief Function to add sql resquest "and condition" to a given request 
   * \fn void addOptionRequest(const std::string& name, const std::string& value, std::string& request) 
   * \param name The column name of the data base table 
   * \param value The value to search in the given column 
   */ 
  void addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
    request.append(" and "+name+"=");
    request.append("'"+value+"'");
  }

  /**
   * \brief Function to add sql resquest "and condition" which contain an integer value to a given request 
   * \fn void addOptionRequest(const std::string& name, T& value, std::string& request) 
   * \param name The column name of the data base table 
   * \param value The integer value to search in the given column 
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
   * \fn void addOptionRequest(const std::string& name, T& value, std::string& request) 
   * \param name The column name of the data base table 
   * \param value The integer value to search in the given column 
   */
  template <class T>
  void addTimeRequest(const std::string& name, T& value, std::string& request, std::string comp) {
    std::ostringstream osValue;
    osValue << value;
    request.append(" and "+name+ " "+comp+" ");
    request.append("'"+osValue.str()+"'");
    std::cout << "**********************request = " << request << std::endl;
    //request.append("'"+osValue.str()+"'");
  }

  /**
   * \brief Function to add sql resquest "where condition" to a given request 
   * \fn void addOptionRequest(const std::string& name, const std::string& value, std::string& request) 
   * \param name The column name of the data base table 
   * \param value The value to search in the given column 
   */
  void addCondition(const std::string& name, const std::string& value, std::string& request) {
    request.append(" where "+name+"=");
    request.append("'"+value+"'");
  }

  /**
   * \brief Function to add sql resquest "where condition" which contain an integer value to a given request 
   * \fn void addOptionRequest(const std::string& name, T& value, std::string& request) 
   * \param name The column name of the data base table 
   * \param value The integer value to search in the given column 
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
   * \fn long convertToTimeType(std::string date) 
   * \param date The date to convert 
   * \return The converted value
   */
  long convertToTimeType(std::string date) {
    
    if(date.size()==0) return 0;
         
      boost::posix_time::ptime pt(time_from_string(date));
      boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
      time_duration::sec_type time = (pt - epoch).total_seconds();
     
      std::cout << "**********TimeValue=" << time_t(time) << "********************" << std::endl;
    return time_t(time);

  }

  /**
   * \brief Function to check if a given user identifier exists 
   * \fn void checkUserId(std::string userId) 
   * \param userId the user identifier 
   * \return raises an exception on error
   */
  void checkUserId(std::string userId) {
    std::string sqlUserRequest = "SELECT userid from users where userid='"+userId+"'";
    DatabaseResult *user = mdatabaseVishnu->getResult(sqlUserRequest.c_str());
    if(user->getNbTuples()==0) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
    }
  }

  /**
   * \brief Function to check if a given machine identifier exists 
   * \fn void checkUserId(std::string machineId) 
   * \param machineId the machine identifier 
   * \return raises an exception on error
   */
  void checkMachineId(std::string machineId) {
    std::string sqlMachineRequest = "SELECT machineid from machine where machineid='"+machineId+"'";
    DatabaseResult *machine = mdatabaseVishnu->getResult(sqlMachineRequest.c_str());
    if(machine->getNbTuples()==0) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
  }

  /**
   * \brief Function to check if a given machine client identifier exists 
   * \fn void checkUserId(std::string clmachineId) 
   * \param clmachineId the machine client identifier 
   * \return raises an exception on error
   */
  void checkClientMachineName(std::string clmachineId) {
    std::string sqlclMachineRequest = "SELECT name from clmachine where name='"+clmachineId+"'";
    DatabaseResult *clmachine = mdatabaseVishnu->getResult(sqlclMachineRequest.c_str());
    if(clmachine->getNbTuples()==0) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
  }

  /**
   * \brief Function to check if a given option name exists 
   * \fn void checkOptionName(std::string name) 
   * \param the name of the option 
   * \return raises an exception on error
   */
  void checkOptionName(std::string name) {
    std::string sqlNameRequest = "SELECT description from optionu where description='"+name+"'";
    DatabaseResult *nameResults = mdatabaseVishnu->getResult(sqlNameRequest.c_str());
    if(nameResults->getNbTuples()==0) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION);
    }
  } 

  /**
   * \brief Function to check if a given session identifier exists 
   * \fn void checkUserId(std::string sessionId) 
   * \param sessionId the session identifier 
   * \return raises an exception on error
   */ 
  void checkSessionId(std::string sessionId) {
    std::string sqlSessionRequest = "SELECT vsessionid from vsession where vsessionid='"+sessionId+"'";
    DatabaseResult *session = mdatabaseVishnu->getResult(sqlSessionRequest.c_str());
    if(session->getNbTuples()==0) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_SESSION_ID);
    }
  }

  /**                         
   * \brief Function to check a status value 
   * \fn void void checkStatus(const int& status) 
   * \param status The status to check
   * \return raises an exception on error
   */
  void checkStatus(const int& status) {
     if((status < 0) || (status > 1)) {
       throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION, "Invalid Status value: its value must be 0 or 1");
     }
  }

  /**                         
   * \brief Function to check a close policy value 
   * \fn void checkClosePolicy(const int& closePolicy) 
   * \param closePolicy The closePolicy value to check
   * \return raises an exception on error
   */
  void checkClosePolicy(const int& closePolicy) {
      if((closePolicy < 0) || (closePolicy > 2)) {
         throw UMSVishnuException(ERRCODE_UNKNOWN_CLOSURE_MODE, "Invalid ClosePolicy value: its value must be 0, 1 or 2");
      }
  }

  /**
   * \fn ~QueryServer()
   * \brief Destructor, raises an exception on error
   */ 
  virtual ~QueryServer()
  {
  }

protected:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The pointer to the QueryParameters information 
  */
  QueryParameters *mparameters;
  /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;
  /**
  * \brief The Object containing query information 
  */
  ListObject* mlistObject;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

};

#endif
