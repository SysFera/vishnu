/* This file is a part of VISHNU.

* Copyright SysFera SAS (2011) 

* contact@sysfera.com

* This software is a computer program whose purpose is to provide 
* access to distributed computing resources.
*
* This software is governed by the CeCILL  license under French law and
* abiding by the rules of distribution of free software.  You can  use, 
* modify and/ or redistribute the software under the terms of the CeCILL
* license as circulated by CEA, CNRS and INRIA at the following URL
* "http://www.cecill.info". 

* As a counterpart to the access to the source code and  rights to copy,
* modify and redistribute granted by the license, users are provided only
* with a limited warranty  and the software's author,  the holder of the
* economic rights,  and the successive licensors  have only  limited
* liability. 
*
* In this respect, the user's attention is drawn to the risks associated
* with loading,  using,  modifying and/or developing or reproducing the
* software by the user in light of its specific status of free software,
* that may mean  that it is complicated to manipulate,  and  that  also
* therefore means  that it is reserved for developers  and  experienced
* professionals having in-depth computer knowledge. Users are therefore
* encouraged to load and test the software's suitability as regards their
* requirements in conditions enabling the security of their systems and/or 
* data to be ensured and,  more generally, to use and operate it in the 
* same conditions as regards security. 
*
* The fact that you are presently reading this means that you have had
* knowledge of the CeCILL license and that you accept its terms.
*/

/**
 * \file ListProgressServer.hpp
 * \brief This file contains the VISHNU ListProgressServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */
#ifndef _LIST_PROGRESS_SERVER_
#define _LIST_PROGRESS_SERVER_

#include <string>
#include <vector>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/local_time/posix_time_zone.hpp>
#include <boost/date_time/date_facet.hpp>
#include <sstream>

#include "QueryServer.hpp"
#include "SessionServer.hpp"
#include "TMS_Data.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"

/**
 * \class ListProgressServer
 * \brief ListProgressServer class implementation
 */
class ListProgressServer: public QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression> {

public:

  /**
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param machineId The identifier of the machine on which the jobs whill be listed  
   * \brief Constructor, raises an exception on error
   */
  ListProgressServer(const SessionServer session, const std::string& machineId):
    QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(session)
  {
   mcommandName = "vishnu_get_job_progress";
   mmachineId = machineId;
  }
  /**
   * \param params The object which encapsulates the information of ListProgressServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \param machineId The identifier of the machine on which the job has been submitted
   * \brief Constructor, raises an exception on error
   */
  ListProgressServer(TMS_Data::ProgressOptions_ptr params, const SessionServer& session, const std::string& machineId):
    QueryServer<TMS_Data::ProgressOptions, TMS_Data::ListProgression>(params, session)
  {
    mcommandName = "vishnu_get_job_progress";
    mmachineId = machineId;
  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  TMS_Data::ListProgression*
  list() {
    
    //To check the sessionKey
    msessionServer.check();

    std::vector<std::string> results;
    std::vector<std::string>::iterator  iter;
    std::string batchJobId;
    int status;
    long startTime;
    long walltime;

    std::string acLogin = UserServer(msessionServer).getUserAccountLogin(mmachineId);

    TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListProgression();

    std::string sqlRequest = "SELECT jobId, jobName, wallClockLimit, endDate, status, batchJobId from vsession, job where"
            " vsession.numsessionid=job.vsession_numsessionid and submitMachineId='"+mmachineId+"'";

    if(mparameters->getJobId().size()!=0) {
      std::string jobId = mparameters->getJobId();
      sqlRequest.append(" and jobId='"+jobId+"'");
      boost::scoped_ptr<DatabaseResult> sqlResult(ServerTMS::getInstance()->getDatabaseVishnu()->getResult(sqlRequest.c_str()));
      if(sqlResult->getNbTuples() == 0) {
        throw TMSVishnuException(ERRCODE_UNKNOWN_JOBID);        
      }
    } else {
      if(mparameters->getJobOwner().size()!=0) {
        acLogin = mparameters->getJobOwner(); 
      }
      sqlRequest.append(" and owner='"+acLogin+"'");
    }

    sqlRequest.append("  and status < 5 order by jobId");
    
    boost::scoped_ptr<DatabaseResult> sqlResult(ServerTMS::getInstance()->getDatabaseVishnu()->getResult(sqlRequest.c_str()));

    if (sqlResult->getNbTuples() != 0){
      for (size_t i = 0; i < sqlResult->getNbTuples(); ++i) {

        results.clear();
        results = sqlResult->get(i);
        iter = results.begin();

        TMS_Data::Progression_ptr job = ecoreFactory->createProgression();

        job->setJobId(*iter);
        job->setJobName(*(++iter));
        walltime = convertToInt(*(++iter));
        job->setWallTime(walltime);
        job->setEndTime(convertToTimeType(*(++iter)));
        status = convertToInt(*(++iter));
        job->setStatus(status);
        batchJobId = *(++iter);    

        BatchFactory factory;
        BatchType batchType  = ServerTMS::getInstance()->getBatchType(); 
        boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchType));

        startTime = batchServer->getJobStartTime(batchJobId); 
        if(startTime!=0) {
          job->setStartTime(startTime);

          if(status==5) {
            job->setPercent(100);  
          } else if(status==4) {
            time_t currentTime = vishnu::getCurrentTimeInUTC();
            int percent = 0;
            time_t gap = currentTime-startTime;
            if(walltime==0) {
               walltime = 60;
            }

            if(gap < walltime) {
              double ratio =  100*(double(gap)/walltime);
              if(ratio > 0.0 && ratio <= 1.0) {
                percent = 1;
              } else {
                percent = static_cast<int>(ratio);
              }
            } else {
              percent = 99;
            }
            job->setPercent(percent);
          } else {
            job->setPercent(0);
          } 
        } else {
          job->setPercent(0);
        }
        mlistObject->getProgress().push_back(job); 
      }
    }
    
    mlistObject->setNbJobs(mlistObject->getProgress().size());
   
    return mlistObject;

  }


  /**
   * \brief Function to get the name of the ListProgressServer command line
   * \return The the name of the ListProgressServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListProgressServer()
  {
  }

  private:

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListProgressServer command line
  */
  std::string mcommandName;
   /**
  * \brief The identifier of the machine in which the jobs whill be listed
  */
  std::string mmachineId; 
};

#endif
