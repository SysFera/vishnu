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


// T2.5 :  list job queues
BOOST_FIXTURE_TEST_SUITE(list_job_queues, TMSSeDFixture)


  //list job queues : normal call

BOOST_AUTO_TEST_CASE(list_job_queues_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the list job queues function corresponding to use case T2.3" );

  try {
    
    //Check the batch type    
    BOOST_CHECK(BATCHTYPE=="TORQUE" || BATCHTYPE=="SLURM" || BATCHTYPE=="LSF" || BATCHTYPE=="SGE");

    VishnuConnexion vc("root","vishnu_user");

    // get the session key and the machine identifier

    string sessionKey=vc.getConnexion();

    string machineId="machine_1";
    string lsfQueuesConfigdir;
    string oldContentLSFQueuesConfigFile;
    // create queues
    std::ostringstream createCommand;
    if(BATCHTYPE=="TORQUE") {
      createCommand << "qmgr";
      createCommand << " " << TMSBATCHHOSTNAME << " < " << TMSCONFIGDIR << "/config_queues_test.cfg >/dev/null";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
    } else if(BATCHTYPE=="SLURM") {
      createCommand << "scontrol create partition=" << "test_queue1";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
      createCommand.str("");
      createCommand << "scontrol create partition=" << "test_queue2";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
    } else if(BATCHTYPE=="LSF") {
      lsfQueuesConfigdir = LSFQUEUESCONFIGDIR;
      oldContentLSFQueuesConfigFile = getFileContent(lsfQueuesConfigdir);
      std::ostringstream addedQueue;
      //add of first queue
      addedQueue << "Begin Queue \n";
      addedQueue << "QUEUE_NAME   = test_queue1\n";
      addedQueue << "PRIORITY     = 10\n";
      addedQueue << "NICE         = 20\n";
      addedQueue << "DESCRIPTION = First vishnu test queue.\n";
      addedQueue << "End Queue \n";
      //add of second queue 
      addedQueue << "Begin Queue \n";
      addedQueue << "QUEUE_NAME   = test_queue2\n";
      addedQueue << "PRIORITY     = 10\n";
      addedQueue << "NICE         = 20\n";
      addedQueue << "DESCRIPTION = Second vishnu test queue.\n";
      addedQueue << "End Queue \n";
      //modify LSF configuration file
      std::ofstream outfile(lsfQueuesConfigdir.c_str(), ios_base::app);
      outfile << addedQueue.str();
      outfile.close();
      //reconfig queues list
      createCommand << "badmin reconfig";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
    } else if(BATCHTYPE=="SGE") {
      createCommand << "qconf -Aq " << TMSCONFIGDIR << "/config_queues_SGE_test1.cfg ";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
      createCommand.str("");
      createCommand << "qconf -Aq " << TMSCONFIGDIR << "/config_queues_SGE_test2.cfg ";
      BOOST_CHECK_EQUAL(system(createCommand.str().c_str()), 0);
    } else {
      BOOST_TEST_MESSAGE("***********************Unknown Batch Type*****************************");  
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, "Unknown Batch Type");
    }

    ListQueues listofQueues;

    BOOST_CHECK_EQUAL( listQueues(sessionKey, machineId, listofQueues),0);
    bool success = false;
    string name;

    // check the successfull of list queues function

    //  bpt::seconds sleepTime(120);

    //boost::this_thread::sleep(sleepTime);


    for (int i=0; i< listofQueues.getQueues().size(); ++i){
      name=listofQueues.getQueues().get(i)->getName();
      if (name=="test_queue1" || name == "test_queue2"){
        success =true ;
      }
    }
    BOOST_CHECK(success);
    //Delete the precedent created queues
    // create queues
    ostringstream delCommand;
    if(BATCHTYPE=="TORQUE") {
      delCommand << "qmgr";
      delCommand << " " << TMSBATCHHOSTNAME <<  " < " << TMSCONFIGDIR << "/config_clean_queues.cfg >/dev/null";
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);
    } else if(BATCHTYPE=="SLURM") {
      delCommand << "scontrol delete partition=" << "test_queue1";
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);
      delCommand.str("");
      delCommand << "scontrol delete partition=" << "test_queue2";
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);
    } else if(BATCHTYPE=="LSF") {
      //restore LSF configuration file
      std::ofstream outfile(lsfQueuesConfigdir.c_str());
      outfile << oldContentLSFQueuesConfigFile;
      outfile.close();
      delCommand << "badmin reconfig" ;
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);
    } else if (BATCHTYPE=="SGE"){
      delCommand << "qconf -dq " << "test_queue1";
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);
      delCommand.str("");
      delCommand << "qconf -dq " << "test_queue2";
      BOOST_CHECK_EQUAL(system(delCommand.str().c_str()), 0);    
    }

    BOOST_TEST_MESSAGE("*********************** list job queues: normal call ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------
//  get Jobs progression: bad parameters : bad sessionKey

BOOST_AUTO_TEST_CASE(list_job_queues_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad sessionKey for the list job queues function corresponding to use case T2.3" );

  string machineId="machine_1";

  ListQueues listofQueues;

  BOOST_CHECK_THROW(listQueues( "bad sessionKey", machineId, listofQueues),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** list job queues : bad sessionKey ok!!!!*****************************");
}


//------------------------------------------------------------------------------------------------------------------------

//  get Jobs progression: bad parameters : bad machineId

BOOST_AUTO_TEST_CASE(list_job_queues_bad_machineId)
{

  BOOST_TEST_MESSAGE(" Testing bad machineId for the list job queues function corresponding to use case T2.3" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  ListQueues listofQueues;

  BOOST_CHECK_THROW(listQueues(sessionKey, "bad machineId", listofQueues),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** list job queues : bad machineId ok!!!!*****************************");
}

BOOST_AUTO_TEST_SUITE_END()


// THE END

