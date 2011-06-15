#include "HM.hpp"
// For the sleep call
#include "unistd.h"

#include "utilServer.hpp"
#include <fstream>

HM::HM(string mail):mms(UserServer(SessionServer("")), mail){

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
  std::ofstream of("HM.log");
  while(true){
    try {
      freq = mms.checkUpFreq();
//       sleep(freq);
      unsigned int t = sleep(10);
      of << t << std::endl;
      li = mms.getCurMet();
      mms.addMetricSet(li, mid);
    } catch (VishnuException &e) {
      // DO nothing to keep on trying to log
    }
  }
  of.close();
}


