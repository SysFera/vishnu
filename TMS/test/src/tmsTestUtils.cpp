#include "tmsTestUtils.hpp"
using namespace std;



VishnuConnexion::VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mco(co){

}

VishnuConnexion::~VishnuConnexion(){
  if(false==msession.getSessionKey().empty()){
  BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
  BOOST_TEST_MESSAGE("The session is closed");
  }
}

string VishnuConnexion::getConnexion(){

  BOOST_REQUIRE(vishnu::connect(muid,mupwd,msession,mco)==0);
   BOOST_TEST_MESSAGE("The session is open");
   return msession.getSessionKey();
}


bool operator== (const Job& lJob,const Job& rJob ){

  return ( (lJob.getJobId() == rJob.getJobId())
        && (lJob.getSubmitMachineId() == rJob.getSubmitMachineId())  
        && (lJob.getJobPath() == rJob.getJobPath()) 
        && (lJob.getJobName() == rJob.getJobName()) 
        && (lJob.getJobPrio() == rJob.getJobPrio())
        && (lJob.getOwner() == rJob.getOwner() ) 
        && (lJob.getJobQueue() == rJob.getJobQueue() ) 
        && (lJob.getWallClockLimit() == rJob.getWallClockLimit() ) 
      //  && (lJob.getSubmitDate() == rJob.getSubmitDate() ) 
     
     
      );



}




