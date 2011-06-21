#include "HM.hpp"
// For the sleep call
#include "unistd.h"

#include "utilServer.hpp"

HM::HM(string mail, string mid):mms(UserServer(SessionServer("")), mail), mmid(mid){

}

HM::~HM(){
}

void
HM::run(){
  int freq;

  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr li = ecoreFactory->createListMetric();
  while(true){
    try {
      freq = mms.checkUpFreq();
      while (freq > 0) {
	freq = sleep(freq);
      }
      li = mms.getCurMet();
      mms.addMetricSet(li, mmid);
    } catch (VishnuException &e) {
      // DO nothing to keep on trying to log
    }
  }
}


