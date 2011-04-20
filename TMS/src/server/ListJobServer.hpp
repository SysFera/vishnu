/**
 * \file ListJobServer.hpp
 * \brief This file contains the VISHNU ListJobServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_JOB_SERVER_
#define _LIST_JOB_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "QueryServer.hpp"
#include "SessionServer.hpp"
#include "TMS_Data.hpp"

/**
 * \class ListJobServer
 * \brief ListJobServer class implementation
 */
class ListJobServer: public QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs> {

public:

  /**
   * \fn ListJobServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListJobServer(const SessionServer session):
    QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>(session)
  {
   mcommandName = "vishnu_list_jobs";
  }
  /**
   * \fn ListJobServer(const UMS_Data::ListSessionOptions_ptr params,
   *                        const SessionServer& session)
   * \param params The object which encapsulates the information of ListJobServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListJobServer(TMS_Data::ListJobsOptions_ptr params, const SessionServer& session):
    QueryServer<TMS_Data::ListJobsOptions, TMS_Data::ListJobs>(params, session)
  {
    mcommandName = "vishnu_list_jobs";
  }

  /**
   * \brief Function to treat the listSessionServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListSessionOptions_ptr& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListSessionServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListSessionOptions_ptr& options, std::string& sqlRequest)
  {

  }

  /**
   * \brief Function to list sessions information
   * \fn UMS_Data::ListSessions* list()
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListJobs*
  list() {
    std::string sqlListOfJobs = "SELECT jobId, jobName from job, vsession "
    "where vsession.numsessionid=job.vsession_numsessionid";

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;

    TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListJobs();

    msessionServer.check();

    boost::scoped_ptr<DatabaseResult> ListOfJobs (mdatabaseVishnu->getResult(sqlListOfJobs.c_str()));

        if (ListOfJobs->getNbTuples() != 0){
        for (size_t i = 0; i < ListOfJobs->getNbTuples(); ++i) {
          results.clear();
          results = ListOfJobs->get(i);
          ii = results.begin();

            TMS_Data::Job_ptr job = ecoreFactory->createJob();
            job->setJobId(*(ii));
            job->setJobName(*(++ii));
            mlistObject->getJobs().push_back(job);
        }
      }
    return mlistObject;
  }


  /**
   * \brief Function to get the name of the ListJobServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListJobServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListJobServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListJobServer()
  {
  }

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListJobServer command line
  */
  std::string mcommandName;

};

#endif
