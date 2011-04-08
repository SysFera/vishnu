#include "displayer.hpp"

using namespace std;

void 
displayAllJobOutput(TMS_Data::ListJobResults& j){
  for (unsigned int i = 0 ; i < j.getResults().size() ; i++){
    displayJobOutput(j.getResults().get(i));
  }
}

void 
displayJobOutput(TMS_Data::JobResult_ptr j){
  cout << " ------------------------ " << endl;
  cout << " ID         : " << j->getJobId() << endl;
  cout << " Output path: " << j->getOutputPath() << endl;
  cout << " Error  path: " << j->getErrorPath() << endl;
  cout << endl;
}

void 
displayJob(TMS_Data::Job_ptr& j){
  cout << " ------------------------ " << endl;

}

