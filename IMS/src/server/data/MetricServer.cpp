#include "MetricServer.hpp"
#include "IMSVishnuException.hpp"
#include "DbFactory.hpp"
#include <vector>
#include "DIET_data.h"
#include "DIET_server.h"

MetricServer::MetricServer(const UserServer session):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}
MetricServer::MetricServer(const UserServer session, IMS_Data::MetricHistOp_ptr op):msession(session){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mhop = op;
}
MetricServer::MetricServer(const UserServer session, IMS_Data::CurMetricOp_ptr op):msession(session){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mcop = op;
}
MetricServer::~MetricServer(){
}

void
MetricServer::setUpFreq(unsigned int freq){
  // TODO FIX MVISHNU ID
  mvishnuId  = 1;
  string request = "update \"vishnu\" * set \"updatefreq\"='"+convertToString(freq)+"' where \"vishnuid\"='";
  request += convertToString(mvishnuId);
  request += "'";
  try {
    mdatabase->process(request.c_str());
  }catch(SystemException& e) {
    e.appendMsgComp("Failed to set frequency to "+convertToString(freq));
    throw(e);
  }
}


void
MetricServer::addMetricSet(IMS_Data::ListMetric* set, string mid){
  string nmid;
  double cpu;
  double mem;
  double disk; 
  // Getting the num machine id to insert
  string reqnmid = "SELECT * from machine where \"machineid\"='"+mid+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqnmid.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown process");
  }
  // numerical index always in position 0 in tables
  nmid = result->get(0).at(0);
  // Filling values (If various in list, the latest is kept
  for (unsigned int i = 0 ; i < set->getMetric().size() ; i++){
    switch (set->getMetric().get(i)->getType()) {
    case 1 : //cpu
      cpu = set->getMetric().get(i)->getValue();
      break;
    case 3 : //disk
      disk = set->getMetric().get(i)->getValue();
      break;
    case 5: //mem
      mem = set->getMetric().get(i)->getValue();
      break;
    default:
      throw SystemException (ERRCODE_SYSTEM, "Unknown metric type");
      break;
    }
  }

  // Inserting the value
  string req = "insert into state(machine_nummachineid, memory, diskspace, cpuload, time) values('"+nmid+"', '"+convertToString(static_cast<int>(mem))+"', '"+convertToString(static_cast<int>(disk))+"', '"+convertToString(static_cast<int>(cpu))+"', CURRENT_TIMESTAMP) ";
  
  try{
    mdatabase->process(req.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error inserting new system states ");
    throw(e);
  }
}

unsigned int
MetricServer::checkUpFreq(){
  // TODO FIX MVISHNU ID
  mvishnuId  = 1;
  // Get the corresponding frequency
  string request = "select * from vishnu where vishnuid='";
  request += convertToString(mvishnuId);
  request += "'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(request.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVVISHNU, "Unknown VISHNU id");
  }
  vector<string> res;
  res = result->get(0);
  // Updating the frequency value
  mfreq = convertToInt(res.at(FREQPOS));
  // Returning the new frequency value
  return mfreq;
}


// TODO SET TIME
IMS_Data::ListMetric*
MetricServer::getCurMet(){
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
  estVector_t vec = diet_new_estVect();
  IMS_Data::Metric_ptr met;
  double disk;
  double cpu;
  double mem;
  time_t time;

  diet_estimate_cori_add_collector(EST_COLL_EASY,NULL);

  diet_estimate_cori (vec, EST_FREESIZEDISK, EST_COLL_EASY, "./");
  disk = diet_est_get_system(vec, EST_FREESIZEDISK, -1); 
  met = ecoreFactory->createMetric();
  met->setType(3);
  met->setValue(static_cast<int>(disk));
  met->setTime(time);
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 5) {
    mlistObject->getMetric().push_back(met);
  }

  diet_estimate_cori (vec, EST_FREEMEM, EST_COLL_EASY, NULL);
  mem = diet_est_get_system(vec, EST_FREEMEM, -1); 
  met = ecoreFactory->createMetric();
  met->setType(5);
  met->setValue(static_cast<int>(mem));
  met->setTime(time);
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 3) {
    mlistObject->getMetric().push_back(met);
  }

  // TODO CHECK WHY -1 GOTTEN
  diet_estimate_cori (vec, EST_AVGFREECPU, EST_COLL_EASY, NULL);
  cpu = diet_est_get_system(vec, EST_FREECPU, -1); 
  met = ecoreFactory->createMetric();
  met->setType(1);
  met->setValue(static_cast<int>(cpu));
  met->setTime(time);
  if (mcop->getMetricType() != 3 && mcop->getMetricType() != 5) {
    mlistObject->getMetric().push_back(met);
  }

  diet_destroy_estVect(vec) ;

  return mlistObject;
}

// TODO: Remove the constant values if possible !!!!!!!!!!!!
IMS_Data::ListMetric*
MetricServer::getHistMet(string machineId){
  string request = "select * from state, machine where machine.machineid='"+machineId+"' AND machine.nummachineid=state.machine_nummachineid";
  vector<string>::iterator iter;
  vector<string> results = vector<string>();

  IMS_Data::MetricType type = mhop->getType();

  // TODO BAD COMPARISON CHANGE IT
  if (mhop->getStartTime()>0) {
    request += " AND EXTRACT( epoch FROM  time ) >";
    request += convertToString(mhop->getStartTime());
  }
  if (mhop->getEndTime()>0) {
    request += " AND EXTRACT( epoch FROM  time ) <";
    request += convertToString(mhop->getEndTime());
  }
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
  try {
    boost::scoped_ptr<DatabaseResult> listOfMetric (mdatabase->getResult(request.c_str()));
    // Do not factorize the code to avoid repeating the 'if' tests in the for
    switch (type) {
    case 1 : // CASE CPU USE
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(1);
	met->setValue(convertToInt(*(iter+4)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
      }
      break;
    case 3: // CASE FREE DISKSPACE
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(3);
	met->setValue(convertToInt(*(iter+3)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
      }
      break;
    case 5: // CASE FREE MEMORY
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(5);
	met->setValue(convertToInt(*(iter+2)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
      }
      break;
    default :
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(5);
	met->setValue(convertToInt(*(iter+2)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(3);
	met->setValue(convertToInt(*(iter+3)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(1);
	met->setValue(convertToInt(*(iter+4)));
	met->setTime(convertToInt(*(iter+5)));
	mlistObject->getMetric().push_back(met);
      }
      break;
    }
  } catch (SystemException& e) {
    throw (e);
  }
  return mlistObject;
}
