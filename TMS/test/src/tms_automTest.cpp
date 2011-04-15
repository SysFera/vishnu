//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include <iostream>
#include "fixtures.hpp"

using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;

BOOST_GLOBAL_FIXTURE(VishnuConfigure)


//BOOST_MESSAGE(" ************************** TMS API Test   ****************************************** ");
// connexion to vishnu


class VishnuConnexion{

public:

//addUser();
//changePassword();
//addMachine();
VishnuConnexion(const string& uid, const string& upwd, const string& mid, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mmid(mid),mco(co){

BOOST_REQUIRE_EQUAL(vishnu::connect(uid,upwd,msession,mco),0);
BOOST_MESSAGE("The session is open");
}

~VishnuConnexion(){
BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
BOOST_MESSAGE("The session is closed");
}

 UMS_Data::Session msession;
 UMS_Data::ConnectOptions mco;
 std::string muid;
 std::string mupwd;
 std::string mmid;

};


//Test category 1

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(submit_a_job)

// submit a job: normal call

BOOST_AUTO_TEST_CASE( submit_a_Job_normal_call)
{
   
// get the session key and the machine identifier
 string sessionKey=vcp.msession.getSessionKey();
 string machineId=vcp.mmid;
 
 BOOST_MESSAGE(" Testing normal job submission corresponding to use case T1.1" );

//Setting submitjob paramters

const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
Job jobInfo;
SubmitOptions options;
   
BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );
BOOST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
}


// submit a job: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_sessionKey)
{

 BOOST_MESSAGE(" Testing bad session Key for job submission (use case T1.1)" );

   BOOST_CHECK_THROW( true,VishnuException );
}

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_machineId)
{
 BOOST_MESSAGE(" Testing bad machine identifier for job submission (use case T1.1)" );

   BOOST_CHECK_THROW( true,VishnuException );
}


// submit a job: bad parameters: bad script

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script)
{
 BOOST_MESSAGE(" Testing bad script for job submission (use case T1.1)" );

   BOOST_CHECK_THROW( true,VishnuException );
}
BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------------------


// T2.1 : cancel a job

BOOST_AUTO_TEST_SUITE(cancel_a_job)

// cancel a job: normal call

BOOST_AUTO_TEST_CASE( cancel_a_job_normal_call)
{
 BOOST_MESSAGE(" Testing normal job cancelling T2.1" );

 //  BOOST_CHECK_EQUAL( cancelJob(sessionKey, machineId, jobId),0  );
}


// cancel a job: bad parameters

BOOST_AUTO_TEST_CASE( cancel_a_job_bad_parameters)
{
 BOOST_MESSAGE(" Testing bad parameters job cancelling T2.1" );

   BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------------------



//==============================================================================================


/*


//Test category 2

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(T1.1_submit_a_job)

// submit a job: normal call

BOOST_AUTO_TEST_CASE( submitJob_normal_call)
{
 BOOST_MESSAGE(" Testing normal job submission T1.1" );

   BOOST_CHECK( true );
}


// submit a job: bad parameters

BOOST_AUTO_TEST_CASE( submitJob_bad_parameters)
{
 BOOST_MESSAGE(" Testing normal job submission T1.1" );

   BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()



//-----------------------------------------------------------------------------------------------

*/
