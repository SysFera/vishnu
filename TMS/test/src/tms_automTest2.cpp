/**
 * \file tms_automTest
 * \brief Contains TMS API test implementation
 * References: VISHNU_D4_1b_TMS-PlanTests
 */


//UMS forward Headers
#include "UMS_Data_forward.hpp"

// TMS forward Headers
#include "TMS_Data_forward.hpp"
#include "TMS_fixtures.hpp"
#include "tmsTestUtils.hpp"
#include "TMS_testconfig.h"

// C++ Headers
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;


// The database, UMS and TMS SeD are launched by TMSSedFixture.


// T2.4 : list jobs

BOOST_FIXTURE_TEST_SUITE(list_jobs, TMSSeDFixture)


  //  list job : normal call
BOOST_AUTO_TEST_CASE(list_job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job firstJob;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath,firstJob,options)==0  );

    BOOST_TEST_MESSAGE("************ The first job identifier is " << firstJob.getJobId() );

    // submit a second job

    Job secondJob;
    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath,secondJob,options)==0  );

    BOOST_TEST_MESSAGE("************ The second job identifier is " << secondJob.getJobId() );


    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    //lsOptions.setJobId(jobInfo.getJobId());
    // retrieve the two last submitted jobs
    bool found= false;
    bool foundFisrtJob= false;
    bool foundSecondJob= false;

    BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );


  // Check the success of listJobs function

    int i=0;

    while ( ( false==found ) && ( i < lsJobs.getNbJobs()) ){

      if(firstJob==*(lsJobs.getJobs().get(i))){
      foundFisrtJob= true;
      }


      if(secondJob==*(lsJobs.getJobs().get(i))){
     foundSecondJob=true;
      }

      found= (foundSecondJob && foundSecondJob);

      i++;
    }



    BOOST_CHECK( found ) ;

    BOOST_TEST_MESSAGE("*********************** list a job info: normal call ok!!!!*****************************");

  //  Clean up: delete the submitted jobs
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, firstJob.getJobId())==0  );
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, secondJob.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------


//  list job : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(list_job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
  // lsOptions.setJobId(jobInfo.getJobId());

    BOOST_CHECK_THROW(listJobs("bad sessionKey ", machineId,lsJobs,lsOptions),VishnuException  );


    BOOST_TEST_MESSAGE("*********************** list a job info: bad sessionkey ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------


//  list job : bad parameters: bad machineId

BOOST_AUTO_TEST_CASE(list_job_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    lsOptions.setJobId(jobInfo.getJobId());

    BOOST_CHECK_THROW(listJobs(sessionKey, "bad machineId",lsJobs,lsOptions),VishnuException );



    BOOST_TEST_MESSAGE("*********************** list a job : bad machine id ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//list job on all machines: normal call
BOOST_AUTO_TEST_CASE(list_jobs_on_all_machines_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list job on all machines function corresponding to use case T2.8" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="all";
  string submitMachineId1="machine_1";
  string submitMachineId2="machine_2";


  try {

    //  Clean up: delete the submitted jobs
    BOOST_REQUIRE(cancelJob(sessionKey, submitMachineId1, "all")==0  );
    BOOST_REQUIRE(cancelJob(sessionKey, submitMachineId2, "all")==0  );

    //Setting submitjob parameters
    const std::string scriptFilePath=TMSSCRIPTPATH;
    SubmitOptions options;
    
    Job machine1FirstJob;
    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId1, scriptFilePath,machine1FirstJob,options)==0  );
    BOOST_TEST_MESSAGE("************ The first job identifier on " << submitMachineId1 <<" is " << machine1FirstJob.getJobId() );
    Job machine1SecondJob;
    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId1, scriptFilePath,machine1SecondJob,options)==0  );
    BOOST_TEST_MESSAGE("************ The second job identifier on " << submitMachineId1 << " is " << machine1SecondJob.getJobId() );

    Job machine2FirstJob; 
    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId2, scriptFilePath,machine2FirstJob,options)==0  );
    BOOST_TEST_MESSAGE("************ The first job identifier on " << submitMachineId2 << " is " << machine2FirstJob.getJobId() );
    Job machine2SecondJob;
    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId2, scriptFilePath,machine2SecondJob,options)==0  );
    BOOST_TEST_MESSAGE("************ The second job identifier on " << submitMachineId2 << " is " << machine2SecondJob.getJobId() );
    Job machine2ThirdJob;
    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId2, scriptFilePath,machine2ThirdJob,options)==0  );
    BOOST_TEST_MESSAGE("************ The second job identifier on " << submitMachineId2 << " is " << machine2ThirdJob.getJobId() );

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    bool found= false;
    bool foundMachine1FisrtJob= false;
    bool foundMachine1SecondJob=false;
    bool foundMachine2FisrtJob= false;
    bool foundMachine2SecondJob=false;
    bool foundMachine2ThirdJob=false;
    BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );

    // Check the success of listJobs on all machines function
    int i=0;
    int nbSubmittedJobsMachine1=0;
    int nbSubmittedJobsMachine2=0;
    int nbJobs =  lsJobs.getNbJobs();
    BOOST_TEST_MESSAGE("************ Number of total jobs :" << nbJobs);

    while ( ( false==found ) && ( i < nbJobs) ){

      if(machine1FirstJob==*(lsJobs.getJobs().get(i))){
        foundMachine1FisrtJob= true;
        if(machine1FirstJob.getSubmitMachineId()==submitMachineId1){
          nbSubmittedJobsMachine1++;
        }
      }

      if(machine1SecondJob==*(lsJobs.getJobs().get(i))){
        foundMachine1SecondJob=true;
        if(machine1FirstJob.getSubmitMachineId()==submitMachineId1){
          nbSubmittedJobsMachine1++;
        }
      }

      if(machine2FirstJob==*(lsJobs.getJobs().get(i))){
        foundMachine2FisrtJob= true;
        if(machine2FirstJob.getSubmitMachineId()==submitMachineId2){
          nbSubmittedJobsMachine2++;
        }
      }

      if(machine2SecondJob==*(lsJobs.getJobs().get(i))){
        foundMachine2SecondJob=true;
        if(machine2SecondJob.getSubmitMachineId()==submitMachineId2){
          nbSubmittedJobsMachine2++;
        }
      }

      if(machine2ThirdJob==*(lsJobs.getJobs().get(i))){
        foundMachine2ThirdJob=true;
        if(machine2ThirdJob.getSubmitMachineId()==submitMachineId2){
          nbSubmittedJobsMachine2++;
        }
      }

      found= (foundMachine1FisrtJob && foundMachine1SecondJob && foundMachine2FisrtJob && foundMachine2SecondJob && foundMachine2ThirdJob);

      i++;
    }

    BOOST_CHECK( found ) ;
    BOOST_CHECK(nbJobs==5 && nbSubmittedJobsMachine1==2 && nbSubmittedJobsMachine2==3);

    BOOST_TEST_MESSAGE("*********************** list jobs on all machines: normal call ok!!!!*****************************");

    //  Clean up: delete the submitted jobs
    BOOST_REQUIRE(cancelJob(sessionKey, submitMachineId1, "all")==0  );
    BOOST_REQUIRE(cancelJob(sessionKey, submitMachineId2, "all")==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


//list jobs on all machines : bad parameters: bad sessionKey
BOOST_AUTO_TEST_CASE(list_job_on_machine_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list jobs on all machine  function corresponding to use case T2.8" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();

  string machineId="all";
  string submitMachineId="machine_1";

  try {
    //Setting submitjob parameters
    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, submitMachineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    ListJobs lsJobs;
    ListJobsOptions lsOptions;

    BOOST_CHECK_THROW(listJobs("bad sessionKey ", machineId,lsJobs,lsOptions),VishnuException  );
    BOOST_TEST_MESSAGE("*********************** list jobs on all machines: bad sessionkey ok!!!!*****************************");

   // Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, submitMachineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//list batch jobs : normal call
BOOST_AUTO_TEST_CASE(list_batch_jobs_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list batch jobs function corresponding to use case T2.4-B2" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  try {

    //  Clean up: delete the submitted jobs
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, "all")==0  );

    //Setting submitjob parameters
    const std::string scriptFilePath=TMSSCRIPTPATH;
    const std::string stdOutResult="stdOutResult_"+machineId;
    BOOST_TEST_MESSAGE("************BATCH TYPE is equal to " << BATCHTYPE);
    std::ostringstream createCommand; 
    if(BATCHTYPE=="TORQUE") {
      createCommand << "qsub " << scriptFilePath << " > " << stdOutResult;
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0); 
    } else if(BATCHTYPE=="SLURM") {
      createCommand << "sbatch " << scriptFilePath << " > " << stdOutResult;
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
    }
  
    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    lsOptions.setBatchJob(true);
    BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );
    bool found= false;
    int i=0;
    std::string line;
    std::string batchJobId;
    std::istringstream isBatchJobId;
    int nbJobs =  lsJobs.getNbJobs();
    BOOST_TEST_MESSAGE("************ Number of total jobs:" << nbJobs);
    while ( ( false==found ) && ( i < nbJobs) ){
      std::ifstream ifile(stdOutResult.c_str());
      if (ifile.is_open()) {
        while (!ifile.eof()) {
          getline(ifile, line);
          isBatchJobId.str(line);
          isBatchJobId >> batchJobId;
          if(line.find(((lsJobs.getJobs().get(i))->getJobId()))!=std::string::npos) {
            BOOST_TEST_MESSAGE("************ batch Job Id info: " << line);
            BOOST_TEST_MESSAGE("************ result Job Id: " << (lsJobs.getJobs().get(i))->getJobId());
            found = true;
            break;
          }
        }
        ifile.close(); 
      }
      i++;
    }

    BOOST_CHECK( found ) ;
    BOOST_TEST_MESSAGE("*********************** list batch jobs: normal call ok!!!!*****************************");
    
    createCommand.str("");
    createCommand << "rm " << stdOutResult;
    BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
