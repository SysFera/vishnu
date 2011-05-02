#include "MetricServer.hpp"
#include "IMSVishnuException.hpp"
#include "DbFactory.hpp"
#include <vector>
#include "DIET_data.h"
#include "DIET_server.h"

MetricServer::MetricServer(const UserServer session):msession(session){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}
MetricServer::MetricServer(const UserServer session, IMS_Data::MetricHistOp op):msession(session){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}
MetricServer::MetricServer(const UserServer session, IMS_Data::CurMetricOp op):msession(session){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}
MetricServer::~MetricServer(){
}
// TODO mvid
int
MetricServer::getUpFreq(){
  string mvid = "1";
  string req = "select * from vishnu where vishnuid='"+mvid+"'";  

  return mfreq;
}
void
MetricServer::setUpFreq(int freq){
  mfreq = freq;
}

// TODO Finish the function
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
  string req = "insert into state(\"machine_nummachineid\", \"memory\", \"diskspace\", \"cpuload\", \"time\") values('"+nmid+"', '"+convertToString(mem)+"', '"+convertToString(disk)+"', '"+convertToString(cpu)+"', CURRENT_TIMESTAMP) ";

  try{
    mdatabase->process(req.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error inserting new system states ");
    throw(e);
  }
}

int
MetricServer::checkUpFreq(){
  // TODO FIX MVISHNU ID
  cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << endl;
  mvishnuId  = 1;
  cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 1" << endl;
  // Get the corresponding frequency
  string request = "select * from vishnu where vishnuid='";
  request += convertToString(mvishnuId);
  request += "'";
  cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 2" << endl;
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(request.c_str()));
  cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 3" << endl;
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVVISHNU, "Unknown VISHNU id");
  }
  vector<string> res;
  cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << endl;
  res = result->get(0);
  cout << " Value gottent: " << res.at(FREQPOS) << endl;
  // Updating the frequency value
  mfreq = convertToInt(res.at(FREQPOS));
  // Returning the new frequency value
  return mfreq;
}

// TODO Recuperer le temps avec boost
IMS_Data::ListMetric*
MetricServer::getCurMet(){
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
  estVector_t vec = diet_new_estVect();
  IMS_Data::Metric_ptr met;
  double res;
  double disk;
  double cpu;
  double mem;
  diet_estimate_cori_add_collector(EST_COLL_EASY,NULL);
  res = diet_estimate_cori (vec, EST_ALLINFOS, EST_COLL_EASY, NULL);

  disk = diet_est_get_system(vec, EST_FREESIZEDISK, -1); 
  met = ecoreFactory->createMetric();
  met->setType(3);
  met->setValue(disk);
  mlistObject->getMetric().push_back(met);

  mem = diet_est_get_system(vec, EST_FREEMEM, -1); 
  met = ecoreFactory->createMetric();
  met->setType(5);
  met->setValue(mem);
  mlistObject->getMetric().push_back(met);

  cpu = diet_est_get_system(vec, EST_FREECPU, -1); 
  met = ecoreFactory->createMetric();
  met->setType(1);
  met->setValue(cpu);
  mlistObject->getMetric().push_back(met);

  diet_destroy_estVect(vec) ;
  return mlistObject;
}

// TODO: Remove the constant values if possible !!!!!!!!!!!!
IMS_Data::ListMetric*
MetricServer::getHistMet(string machineId){
  string request = "select * from state where \"machineid\"='"+machineId+"'";
  vector<string>::iterator iter;
  vector<string> results = vector<string>();

  IMS_Data::MetricType type = mhistOp.getType();

  if (mhistOp.getStartTime()>0) {
    request += " AND \"timestamp\">'";
    request += convertToString(mhistOp.getStartTime());
    request += "'";
  }
  if (mhistOp.getEndTime()>0) {
    request += " AND \"timestamp\"<'";
    request += convertToString(mhistOp.getEndTime());
    request += "'";
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
