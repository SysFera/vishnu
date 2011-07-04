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
  string unity;
  switch(m->getType()){
  case 1 :
    type = "Free CPU";
    unity = " %";
    break;
  case 2 :
    type = "Free diskspace";
    unity = " Mb";
    break;
  case 3 :
    type = "Free memory";
    unity = " Mb";
    break;
  default:
    type = "unknown ";
    break;
  }
  cout << " ------------------------ " << endl;
  cout << " type : " << type << endl;
  cout << " value: " << m->getValue() << unity << endl;
  string date = boost::posix_time::to_simple_string(boost::posix_time::from_time_t(convertUTCtimeINLocaltime(m->getTime())));
  cout << " time : " << date << endl;
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
  string res = "";
  switch(metricType) {
    case 0:
      res = "UNDEFINED";
    case 1:
      res = "CPUUSE";
    case 2:
      res = "FREEDISKSPACE";
    case 3:
      res = "FREEMORY";
    default:
      res = "UNDEFINED";
  }
  return res;
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

