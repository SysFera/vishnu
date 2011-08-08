#include "UMS_Data_forward.hpp"
#include "TMS_Data_forward.hpp"
#include <iostream>
#include "api_ums.hpp"
#include "api_tms.hpp"
#include  "TMS_testconfig.h"
#include "diet_config_tests.h"
using namespace std;
using namespace UMS_Data;
using namespace vishnu;

void stress(int cpt,const string& sessionKey, const string& machineId ){

  const std::string scriptFilePath=TMSSCRIPTPATH;

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");

  Job jobInfo;

  SubmitOptions subOptions;

  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);

  for (int i=0;i<5;i++){

    cout << " In loop : " <<  i << endl;

    ListQueues listofQueues;

    listQueues(sessionKey, machineId, listofQueues);

  }
  vishnuFinalize();
}


void forker(int cpt,const string& sessionKey, const string& machineId  ){


 // string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");

  cpt--;

  switch(fork()){
    case 0 :
      
      //vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
     
      stress( cpt,sessionKey,machineId) ;

     // vishnuFinalize();

      break;
    case -1 :
      cout << " Fork number " << cpt << " failed " << endl;
      break;
    default :
      if(cpt>0)
        forker(cpt,sessionKey,machineId);
      break;
  }
  return;
}


int main(int argc, char** argv){
  int cpt;
  int i;
  string             	  key  = ""       ;
  string 	     	  pwd  = "vishnu_user"  ;
  string 	     	  uid  = "root";
  ConnectOptions 	  cop  ;
  Session                 sess ;

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");

  cop.setClosePolicy(2);  

  try{

    vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
    connect    (uid, pwd, sess, cop );
  }
  catch(VishnuException& e){

    std::cout << e.what() << std::endl;

    return 1;
  }
  vishnuFinalize();

  if (argc>1){
    cpt = atoi (argv[1]);
  }
  else{
    cpt = 5;
  }

  std::cout << "sess.getSessionKey()=" << sess.getSessionKey() << std::endl;
  forker(cpt, sess.getSessionKey(),"machine_1");

  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);

  //close(sess.getSessionKey());

  vishnuFinalize();

  return 0;
}


