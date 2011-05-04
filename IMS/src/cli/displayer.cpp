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
  cout << " ------------------------ " << endl;
  cout << " type : " << m->getType() << endl;
  cout << " value: " << m->getValue() << endl;
  cout << " time : " << m->getTime() << endl;
}

void
displayFreq(int freq) {
  cout << "The frequency of update of the history maker is " << freq << endl;
}
