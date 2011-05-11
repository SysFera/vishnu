#include "utilVishnu.hpp"
#include "displayer.hpp"

using namespace std;
using namespace vishnu;

void
displayListProc(IMS_Data::ListProcesses* li){
  for (unsigned int i = 0 ; i < li->getProcess().size() ; i++){
    displayProc(li->getProcess().get(i));
  }
}

void
displayProc(IMS_Data::Process* p){
  cout << " ------------------------ " << endl;
  cout << " Vishnu name : " << p->getProcessName() << endl;
  cout << " Diet id     : " << p->getDietId() << endl;
  cout << " Machine id  : " << p->getMachineId() << endl;
}

void
displayListMetric(IMS_Data::ListMetric* li){
  for (unsigned int i = 0 ; i < li->getMetric().size() ; i++){
    displayMetric(li->getMetric().get(i));
  }
}

void
displayMetric(IMS_Data::Metric* m){
  string type;
  switch(m->getType()){
  case 1 :
    type = "CPU use ";
    break;
  case 3 :
    type = "Diskspace used ";
    break;
  case 5 :
    type = "Memory used ";
    break;
  default:
    type = "unknown ";
  }
  cout << " ------------------------ " << endl;
  cout << " type : " << type << endl;
  cout << " value: " << m->getValue() << endl;
  cout << " time : " << m->getTime() << endl;
}

void
displayFreq(int freq) {
  cout << "The frequency of update of the history maker is " << freq << endl;
}


void
displaySystemInfo(IMS_Data::SystemInfo* sysInfo){
  cout << " ------------------------ " << endl;
  cout << " machineId : " << sysInfo->getMachineId() << endl;
  cout << " Disk space: " << sysInfo->getDiskSpace() << endl;
  cout << " Memory : " << sysInfo->getMemory() << endl;
}


void
displayListSysInfo(IMS_Data::ListSysInfo* li) {
  for (unsigned int i = 0 ; i < li->getSysInfo().size() ; i++){
    displaySystemInfo(li->getSysInfo().get(i));
  }
}


/**
 * \brief  function to convert the type of metric into string
 * \param metricType: The type of metric
 * \return The converted state value
 */
std::string
convertMetricToString(const int& metricType) {

  switch(metricType) {
    case -1:
      return "ALL";
    case 0:
      return "UNDEFINED";
    case 1:
      return "CPUUSE";
    case 2:
      return "DISKSPACE";
    case 3:
      return "FREEDISKSPACE";
    case 4:
      return "MEMORY";
    case 5:
      return "FREEMORY";
    case 6:
      return "CPUNBR";
    default:
      return "UNDEFINED";
  }
}



void
displaySystemThreshold(IMS_Data::Threshold* systemThreshold) {
  cout << " ------------------------ " << endl;
  cout << " machineId : " << systemThreshold->getMachineId() << endl;
  cout << " value : " << systemThreshold->getValue() << endl;
  cout << " handler : " << systemThreshold->getHandler() << endl;
  cout << " type : " << convertMetricToString(systemThreshold->getType()) << endl;
}


void
displayListSysThreshold(IMS_Data::ListThreshold* li) {
  for (unsigned int i = 0 ; i < li->getThreshold().size() ; i++){
    displaySystemThreshold(li->getThreshold().get(i));
  }
}

