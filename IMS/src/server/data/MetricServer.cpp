#include "MetricServer.hpp"
#include "IMSVishnuException.hpp"

MetricServer::MetricServer(const UserServer session):msession(session){
}
MetricServer::MetricServer(const UserServer session, IMS_Data::MetricHistOp op):msession(session){
}
MetricServer::MetricServer(const UserServer session, IMS_Data::CurMetricOp op):msession(session){
}
MetricServer::~MetricServer(){
}
int
MetricServer::getUpFreq(){
  return mfreq;
}
void
MetricServer::setUpFreq(int freq){
  mfreq = freq;
}

int
MetricServer::checkUpFreq(){
  // Get the corresponding frequency
  string request = "select * from vishnu where vishnuid='";
  request += mvishnuId;
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

IMS_Data::ListMetric*
MetricServer::getCurMet(){
  return NULL;
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
