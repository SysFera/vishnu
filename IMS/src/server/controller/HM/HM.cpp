#include "HM.hpp"
// For the sleep call
#include "unistd.h"

#include "utilServer.hpp"

HM::HM():mms(UserServer(SessionServer(""))){
  
}

HM::~HM(){
}

void
HM::run(){
  int freq;
  // Hostname limit size of 200
  char hname[200];
  gethostname(hname, 199);
  string mid = getMidFromHost(hname);

  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr li = ecoreFactory->createListMetric();

  while(true){
    freq = mms.checkUpFreq();
    sleep(freq);
    li = mms.getCurMet();
    mms.addMetricSet(li, mid);
  }
}


