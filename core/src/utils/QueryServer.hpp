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
#include "DbFactory.hpp"
#include "UMSVishnuException.hpp"
#include "TMSVishnuException.hpp"

/**
 * \class QueryServer
 * \brief QueryServer class implementation
 */
template <class QueryParameters, class ListObject>
class QueryServer
{
public:

   /**
   * \fn QueryServer(const SessionServer& session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  explicit
  QueryServer(const SessionServer& session):msessionServer(session)
  {
    mlistObject = NULL;
    DbFactory factory;
    mdatabaseVishnu = factory.getDatabaseInstance();
  }

  /**
   * \fn QueryServer(QueryParameters* params,
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
   * \brief Destructor, raises an exception on error
   */
  virtual ~QueryServer()
  {
  }

protected:
  /**
   * \brief Function to add sql resquest "and condition" to a given request
   * \fn void addOptionRequest(const std::string& name, const std::string& value, std::string& request)
   * \param name The column name of the data base table
   * \param value The value to search in the given column
   * \param request The request
   */
  void addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
    request.append(" and "+name+"=");
    request.append("'"+value+"'");
  }

  /**
   * \brief Function to add sql resquest "and condition" which contain an integer value to a given request
   * \fn void addIntegerOptionRequest(const std::string& name, T& value, std::string& request)
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
   * \fn void addTimeRequest(const std::string& name, T& value, std::string& request, std::string comp)
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
   * \fn void addCondition(const std::string& name, const std::string& value, std::string& request)
   * \param name The column name of the data base table
   * \param value The value to search in the given column
   * \param request The request
   */
  void addCondition(const std::string& name, const std::string& value, std::string& request) {
    request.append(" where "+name+"=");
    request.append("'"+value+"'");
  }

  /**
   * \brief Function to add sql resquest "where condition" which contain an integer value to a given request
   * \fn void addIntegerCondition(const std::string& name, T& value, std::string& request)
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
   * \fn long long convertToTimeType(std::string date)
   * \param date The date to convert
   * \return The converted value
   */
  long long convertToTimeType(std::string date) {
    if(date.size()==0 ||
        // For mysql, the empty date is 0000-00-00, not empty, need this test to avoid problem in ptime
        date.find("0000-00-00")!=std::string::npos) {
      return 0;
    }

    boost::posix_time::ptime pt(time_from_string(date));
    boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
    time_duration::sec_type time = (pt - epoch).total_seconds();

    return (long long) time_t(time);

  }

  /**
   * \brief Function to check if a given user identifier exists
   * \fn void checkUserId(std::string userId)
   * \param userId the user identifier
   * \return raises an exception on error
   */
  void checkUserId(std::string userId) {

#ifdef USE_SOCI_ADVANCED
	  std::string sqlUserRequest = "SELECT userid from users where userid=:userid";
	  SOCISession ss = mdatabaseVishnu->getSingleSession();
	  std::string DBuserID;
	  ss.execute(sqlUserRequest).into(DBuserID).use(userId);
	  bool gotUser=ss.got_data();
	  mdatabaseVishnu->releaseSingleSession(ss);
	  if(! gotUser) {
		  throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
	  }

#else
    std::string sqlUserRequest = "SELECT userid from users where userid='"+userId+"'";
    boost::scoped_ptr<DatabaseResult> user(mdatabaseVishnu->getResult(sqlUserRequest.c_str()));
    if(user->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
    }
#endif
  }

  /**
   * \brief Function to check if a given machine identifier exists
   * \fn void checkMachineId(std::string machineId)
   * \param machineId the machine identifier
   */
  void checkMachineId(std::string machineId) {
#ifdef USE_SOCI_ADVANCED

	  std::string sqlMachineRequest = "SELECT machineid from machine where machineid=:machineid";
	  SOCISession ss = mdatabaseVishnu->getSingleSession();
	  std::string id;
	  ss.execute(sqlMachineRequest).into(id).use(machineId);
	  bool gotMachine=ss.got_data();
	  mdatabaseVishnu->releaseSingleSession(ss);
	  if(! gotMachine) {
		  throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
	  }

#else
    std::string sqlMachineRequest = "SELECT machineid from machine where machineid='"+machineId+"'";
    boost::scoped_ptr<DatabaseResult> machine(mdatabaseVishnu->getResult(sqlMachineRequest.c_str()));
    if(machine->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
#endif
  }

  /**
   * \brief Function to check if a given machine client identifier exists
   * \fn void checkClientMachineName(std::string clmachineId)
   * \param clmachineId the machine client identifier
   */
  void checkClientMachineName(std::string clmachineId) {
#ifdef USE_SOCI_ADVANCED

	  std::string sqlclMachineRequest = "SELECT name from clmachine where name=:name";
	  SOCISession ss = mdatabaseVishnu->getSingleSession();
	  std::string name;
	  ss.execute(sqlclMachineRequest).into(name).use(clmachineId);
	  bool gotMachine=ss.got_data();
	  mdatabaseVishnu->releaseSingleSession(ss);
	  if(! gotMachine) {
		  throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
	  }

#else
    std::string sqlclMachineRequest = "SELECT name from clmachine where name='"+clmachineId+"'";
    boost::scoped_ptr<DatabaseResult> clmachine(mdatabaseVishnu->getResult(sqlclMachineRequest.c_str()));
    if(clmachine->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
    }
#endif
  }

  /**
   * \brief Function to check if a given option name exists
   * \fn void checkOptionName(std::string name)
   * \param name The name of the option
   * \return raises an exception on error
   */
  void checkOptionName(std::string name) {
#ifdef USE_SOCI_ADVANCED

	  std::string sqlNameRequest = "SELECT description from optionu where description=:name";
	  SOCISession ss = mdatabaseVishnu->getSingleSession();
	  std::string description;
	  ss.execute(sqlNameRequest).into(description).use(name);
	  bool gotOption=ss.got_data();
	  mdatabaseVishnu->releaseSingleSession(ss);
	  if(! gotOption) {
		  throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION);
	  }

#else
    std::string sqlNameRequest = "SELECT description from optionu where description='"+name+"'";
    boost::scoped_ptr<DatabaseResult> nameResults(mdatabaseVishnu->getResult(sqlNameRequest.c_str()));
    if(nameResults->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION);
    }
#endif
  }

  /**
   * \brief Function to check if a given session identifier exists
   * \fn void checkSessionId(std::string sessionId)
   * \param sessionId the session identifier
   */
  void checkSessionId(std::string sessionId) {
#ifdef USE_SOCI_ADVANCED
	  std::string sqlSessionRequest="SELECT vsessionid from vsession where vsessionid=:sessionID";
	  SOCISession sess = mdatabaseVishnu->getSingleSession();
	  std::string vssessionid;
	  sess.execute(sqlSessionRequest).use(sessionId).into(vssessionid);
	  bool gotSession=sess.got_data();
	  mdatabaseVishnu->releaseSingleSession(sess);
	  if(!gotSession)
	  {
		  throw UMSVishnuException(ERRCODE_UNKNOWN_SESSION_ID);
	  }
#else
    std::string sqlSessionRequest = "SELECT vsessionid from vsession where vsessionid='"+sessionId+"'";
    boost::scoped_ptr<DatabaseResult> session(mdatabaseVishnu->getResult(sqlSessionRequest.c_str()));
    if(session->getNbTuples()==0) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_SESSION_ID);
    }
#endif
  }

  /**
   * \brief Function to check a status value
   * \fn void void checkStatus(const int& status)
   * \param status The status to check
   */
  void checkStatus(const int& status) {
    if((status < -1) || (status > 1)) {
      throw UMSVishnuException(ERRCODE_UNKNOWN_OPTION, "Invalid value: its value must be 0 or 1");
    }
  }

  /**
   * \brief Function to check a close policy value
   * \fn void checkClosePolicy(const int& closePolicy)
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
#ifdef USE_SOCI_ADVANCED
	  std::string sqlJobRequest= "SELECT numjobid from job where jobId=:jobId";
	  std::string numjobid;
	  SOCISession sess=mdatabaseVishnu->getSingleSession();
	  sess.execute(sqlJobRequest).use(jobId).into(numjobid);
	  bool gotJob=sess.got_data();
	  mdatabaseVishnu->releaseSingleSession(sess);
	  if(!gotJob)
	  {
		  throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
	  }
#else
      std::string sqlJobRequest = "SELECT numjobid from job where jobId='"+jobId+"'";
      boost::scoped_ptr<DatabaseResult> result (mdatabaseVishnu->getResult(sqlJobRequest.c_str()));
      if(result->getNbTuples() == 0) {
        throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);
      }
#endif
    }

    /**
   * \brief Function to check if a given authSystem identifier exists
   * \param authSystemId the authSystem identifier
   */
  void
    checkAuthSystemId(std::string authSystemId) {
#ifdef USE_SOCI_ADVANCED
      std::string sqlJobRequest = "SELECT authsystemid from authsystem where "
    		  "authsystemid=:auth";
      std::string ret;
      SOCISession sess=mdatabaseVishnu->getSingleSession();
      sess.execute(sqlJobRequest).into(ret).use(authSystemId);
      bool gotAuth=sess.got_data();
      mdatabaseVishnu->releaseSingleSession(sess);
      if(!gotAuth)
      {
    	  throw TMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM);
      }
#else
      std::string sqlJobRequest = "SELECT authsystemid from authsystem where authsystemid='"+authSystemId+"'";
      boost::scoped_ptr<DatabaseResult> result (mdatabaseVishnu->getResult(sqlJobRequest.c_str()));
      if(result->getNbTuples() == 0) {
        throw TMSVishnuException(ERRCODE_UNKNOWN_AUTH_SYSTEM);
      }
#endif
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
  SOCIDatabase *mdatabaseVishnu;

};

#endif
