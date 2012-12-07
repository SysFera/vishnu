#include "UMS_Data_forward.hpp"
#include "IMS_Data_forward.hpp"
#include <iostream>
#include <unistd.h>
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "IMS_testconfig.h"
#include "diet_config_tests.h"

using namespace std;
using namespace UMS_Data;
using namespace vishnu;

void stress(int cpt,const string& sessionKey, const string& machineId, int type){

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
  IMS_Data::CurMetricOp op;
  IMS_Data::MetricHistOp histOp;
  IMS_Data::ListMetric list;

  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
  if (type == 1) {
    for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;
      getMetricCurrentValue(sessionKey, machineId, list, op);
    }
  }
  else {
    for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;
      getMetricHistory(sessionKey, machineId, list, histOp);
    }
  }
  vishnuFinalize();
}


void forker(int cpt,const string& sessionKey, const string& machineId, int type){
  cpt--;

  switch(fork()){
    case 0 :
      stress( cpt,sessionKey,machineId, type) ;
      break;
    case -1 :
      cout << " Fork number " << cpt << " failed " << endl;
      break;
    default :
      if(cpt>0)
        forker(cpt,sessionKey,machineId, type);
      break;
  }
  return;
}


int main(int argc, char** argv){
  int cpt;
  int type;
  string key  = ""       ;
  string pwd  = "vishnu_user"  ;
  string uid  = "root";
  ConnectOptions cop  ;
  Session sess ;

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
  cop.setClosePolicy(2);

  try{
    vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
    connect    (uid, pwd, sess, cop );
    vishnuFinalize();

    if (argc != 3) {
      std::cerr << "Usage: ./ims_stress Nbtests Type "<< std::endl;
      std::cerr << "[1 for getMetricCurrentValue and 2 for getMetricHistory]" << std::endl;
      return 1;
    }
    cpt = atoi (argv[1]);
    type = atoi (argv[2]);
    //To check the cpt value
    if (cpt < 1) {
       std::cerr << "The value of the number of test is incorrect" << std::endl;
       return 1;
    }
    //To check the type value
    if ((type != 1) && (type != 2)) {
       std::cerr << "The value of the type is incorrect" << std::endl;
       return 1;
    }
    std::cout << "sess.getSessionKey()=" << sess.getSessionKey() << std::endl;
    forker(cpt, sess.getSessionKey(),"machine_1", type);
  }
  catch(VishnuException& e){
    std::cout << e.what() << std::endl;
    return 1;
  }
  catch(std::exception& excp){
    std::cout << excp.what() << std::endl;
    return 1;
  }

  return 0;
}
