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


//Test category 1

// T1.1 : submit a job


BOOST_FIXTURE_TEST_SUITE(submit_a_job, TMSSeDFixture)

// submit a job: normal call
BOOST_AUTO_TEST_CASE(submit_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal job submission corresponding to use case T1.1");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath= TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions subOptions;

    BOOST_TEST_MESSAGE("************ The script is " << scriptFilePath );

    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    // To check the success of submitJob function

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    lsOptions.setJobId(jobInfo.getJobId());
    BOOST_REQUIRE(listJobs(sessionKey, machineId,lsJobs,lsOptions)==0  );

    BOOST_REQUIRE( (lsJobs.getJobs().size()==1) && ( *(lsJobs.getJobs().get(0)) == jobInfo )  );

    BOOST_TEST_MESSAGE("***********************  submit a job: normal call   ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// submit a job: normal call
BOOST_AUTO_TEST_CASE(submit_a_Job_normal_call2)
{

  BOOST_TEST_MESSAGE("Testing normal job submission corresponding to use case T1.1");
  BOOST_TEST_MESSAGE("In case where the script contains the vishnu JOB OUTPUT ENVIRONMENT VARIABLES");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSFASTSCRIPTPATH;

    //To get the content of the script
    std::string scriptContent;

    BOOST_TEST_MESSAGE("************ The Script is " << scriptFilePath );

    Job jobInfo;
    SubmitOptions subOptions;
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    Job job;
    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);

    while (5!=job.getStatus()){

      bpt::seconds sleepTime(5);
      boost::this_thread::sleep(sleepTime);

      getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    }
    
    JobResult outputInfos;
    BOOST_CHECK_EQUAL(getJobOutput(sessionKey,machineId, jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),0  );
    BOOST_TEST_MESSAGE("************ outputInfos.getOutputPath() = " << outputInfos.getOutputPath());
    std::string jobOutputPath = outputInfos.getOutputPath();
     //To get the content of the output
    std::string jobOutputContent;
    std::string vishnuEnvId;
    std::string batchEnvId;
    std::string vishnuEnvName;
    std::string batchEnvName;


    std::string envSubmitMachine;
    std::ifstream ifile (jobOutputPath.c_str());
    if (ifile.is_open()) {
      std::ostringstream oss;
      oss << ifile.rdbuf();
      jobOutputContent = oss.str();
      ifile.close();
            
      vishnuEnvId = findValue(jobOutputContent, "#TEST_JOB_ID:");
      batchEnvId = findValue(jobOutputContent, "#BATCH_JOB_ID:");
      vishnuEnvName  = findValue(jobOutputContent, "#TEST_JOB_NAME:");
      batchEnvName  = findValue(jobOutputContent, "#BATCH_JOB_NAME:");

      /* iThe following tests below are difficult: because ALL batchs do not provide environment variable for
       * the number of nodes and the nodes file
      */
      /*
         std::string tmp = findValue(jobOutputContent, "#TEST_NUM_NODES:");
         if(!tmp.empty()) {
         std::istringstream iss(tmp);
         iss >> envNbNodes;
         }
         vishnuNodesFile = findValue(jobOutputContent, "#TEST_NODEFILE:");
         */

      /* Only VISHNU provides this variable: VISHNU_SUBMIT_MACHINE_NAME
       */
     envSubmitMachine = findValue(jobOutputContent, "TEST_SUBMIT_MACHINE_NAME:");

    }

    BOOST_TEST_MESSAGE("*********************** vishnuEnvId=" << vishnuEnvId);
    BOOST_TEST_MESSAGE("*********************** vishnuEnvName=" << vishnuEnvName);
    BOOST_TEST_MESSAGE("*********************** envSubmitMachine=" << envSubmitMachine);

    BOOST_TEST_MESSAGE("*********************** batchEnvId=" << batchEnvId);
    BOOST_TEST_MESSAGE("*********************** batchEnvName=" << batchEnvName);
    BOOST_TEST_MESSAGE("*********************** jobSubmitMachineName=" << job.getSubmitMachineName());

    BOOST_REQUIRE((vishnuEnvId==batchEnvId) && (vishnuEnvName==batchEnvName) && (envSubmitMachine==job.getSubmitMachineName()));

    BOOST_TEST_MESSAGE("***********************  submit a job: normal call2   ok!!!!*****************************");

    bool pathExist=bfs::exists(bfs::path(outputInfos.getOutputPath())) &&  bfs::exists(bfs::path(outputInfos.getOutputPath()));

    bfs::remove (bfs::path(outputInfos.getOutputPath()));
    bfs::remove (bfs::path(outputInfos.getErrorPath()));

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

// submit a job: normal call
BOOST_AUTO_TEST_CASE(submit_a_Job_normal_call3)
{

  BOOST_TEST_MESSAGE("Testing normal job submission corresponding to use case T1.1");
  BOOST_TEST_MESSAGE("By activating an option: working directory");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath= TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions subOptions;
    Job job;
    //First test of option WORKINGDIR
    subOptions.setWorkingDir(TMSWORKINGDIR);
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    BOOST_TEST_MESSAGE("************ First test: job.getJobWorkingDir()=" << job.getJobWorkingDir());
    BOOST_REQUIRE(job.getJobWorkingDir()==TMSWORKINGDIR);
    //Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

    //Second test of option WORKINGDIR
    subOptions.setWorkingDir("/tmp");
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    BOOST_TEST_MESSAGE("************ Second test: job.getJobWorkingDir()=" << job.getJobWorkingDir());
    BOOST_REQUIRE(job.getJobWorkingDir()=="/tmp");
    //Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );


    //Third test of option WORKINGDIR
    subOptions.setWorkingDir(std::string(getenv("HOME")));
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    BOOST_TEST_MESSAGE("************ Third test: job.getJobWorkingDir()=" << job.getJobWorkingDir());
    BOOST_REQUIRE(job.getJobWorkingDir()==std::string(getenv("HOME")));
    //Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

    BOOST_TEST_MESSAGE("***********************  submit a job: normal call3   ok!!!!*****************************");

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------
// submit a job: bad parameters: bad sessionKey
BOOST_AUTO_TEST_CASE(submit_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad session Key for job submission (use case T1.1)");


  // set the machine ID
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTPATH;
  Job jobInfo;
  SubmitOptions subOptions;
  BOOST_CHECK_THROW(submitJob("bad sessionKey", machineId, scriptFilePath, jobInfo,subOptions) ,VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad sessionKey    ok!!!!*****************************");


}

//---------------------------------------------------------------------------

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE("Testing bad machine identifier for job submission (use case T1.1)");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key

  string sessionKey=vc.getConnexion();


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTPATH;
  Job jobInfo;
  SubmitOptions subOptions;
  BOOST_CHECK_THROW( submitJob(sessionKey, "bad machineId", scriptFilePath, jobInfo,subOptions), VishnuException);

  BOOST_TEST_MESSAGE("***********************  submit a job: bad machine ID ok!!!!*****************************");


}

// submit a job: bad parameters: bad script content

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_script_content)
{
  BOOST_TEST_MESSAGE("Testing bad script content for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters
  std::string scriptFilePath;
  if(BATCHTYPE=="TORQUE") {
    scriptFilePath=TMSCONFIGDIR "/bad_torque_script";
  } else if(BATCHTYPE=="SLURM") {
    scriptFilePath=TMSCONFIGDIR "/bad_slurm_script";
  } else if(BATCHTYPE=="LSF") {
    scriptFilePath=TMSCONFIGDIR "/bad_lsf_script";
  } else {
    scriptFilePath=TMSCONFIGDIR "/bad_unknow_batch_type_script";
  }

  Job jobInfo;
  SubmitOptions options;

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad script content  ok!!!!*****************************");


}

// submit a job: bad parameters: bad script path

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_script_path)
{
  BOOST_TEST_MESSAGE("Testing bad script path for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions subOptions;

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),VishnuException );

}


// automatic submit a job: normal call
BOOST_AUTO_TEST_CASE(automatic_submit_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal automatic job submission corresponding to use case T1.2");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string autom="autom";
  string machineId1="machine_1";
  string machineId2="machine_2";

  try {
    //Setting submitjob parameters

    // To cancel all jobs on machine_1 and machine_2
    BOOST_REQUIRE(cancelJob(sessionKey, machineId1, "all") == 0  );
    BOOST_REQUIRE(cancelJob(sessionKey, machineId2, "all") == 0  );

    const std::string scriptFilePath= TMSSCRIPTPATH;
    const std::string scriptFilePathWaiting= TMSSCRIPTPATHWAITING;

    Job jobInfo;
    Job jobInfo2;
    SubmitOptions subOptions;
    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId1, scriptFilePathWaiting, jobInfo,subOptions),0 );
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
    
    BOOST_CHECK_EQUAL(submitJob(sessionKey, autom, scriptFilePath, jobInfo2, subOptions),0 );
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo2.getJobId() );

    BOOST_TEST_MESSAGE("************ Submit machine Id " << jobInfo2.getSubmitMachineId() );
    BOOST_CHECK_EQUAL(jobInfo2.getSubmitMachineId().compare(machineId2), 0);
    // To check the success of submitJob function

    BOOST_TEST_MESSAGE("*********************** automatic submit a job: normal call   ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId1, "all")==0  );
    BOOST_REQUIRE(cancelJob(sessionKey, machineId2, "all")==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//TODO: Est ce que les cas E1, E2 et E3 sont utiles? Parce qu'ils sont déjà testés plus haut?
//autom c'est juste le nom de la machine qui change
//Seul le cas E4 est pertinent

// automatic submit a job: automatic job submission for user with no local accounts
BOOST_AUTO_TEST_CASE(automatic_submit_a_Job_with_user_no_local_accounts)
{

  BOOST_TEST_MESSAGE("Testing automatic job submission for user with no local accounts (use case T1.2-E4)");

  VishnuConnexion vc("user_2","toto");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();

  string autom="autom";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath= TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions subOptions;

    
    BOOST_CHECK_THROW(submitJob(sessionKey, autom, scriptFilePath, jobInfo,subOptions) ,VishnuException );
    BOOST_TEST_MESSAGE("*********************** automatic submit job for user with no local accounts  ok!!!!*****************************");

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------------------

