#include <iostream>
#include <sys/wait.h>

#include "UMS_Data_forward.hpp"
#include "TMS_Data_forward.hpp"

#include "api_ums.hpp"
#include "api_tms.hpp"
#include  "TMS_testconfig.h"
#include "diet_config_tests.h"
using namespace std;
using namespace UMS_Data;
using namespace vishnu;

/**
* \enum BatchType
* \brief The type of the Batch
*/
typedef enum {
  SUBMITJOB,/*!< For submitJob function name */
  LISTJOBS,/*!< For listJobs function name */
  LISTQUEUES, /*!< For listQueues function name */
  UNDEFINED /*!< For UNDEFINED function name */
} FunctionNameType;

FunctionNameType convertToFunctionNameType(const std::string& functionToCallName) {

  if(functionToCallName=="SUBMITJOB") {
    return SUBMITJOB;
  } else if (functionToCallName=="LISTJOBS") {
    return LISTJOBS;
  } else if(functionToCallName=="LISTQUEUES") {
    return LISTQUEUES;
  } else {
    return UNDEFINED;
  }

}

void stress(int cpt,const string& sessionKey, const string& machineId, const FunctionNameType& functNameType){

  const std::string scriptFilePath=TMSSCRIPTPATH;

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");

  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);

  for (int i=0;i<5;i++){
    Job jobInfo;
    ListJobs listOfJobs;
    ListQueues listOfQueues;
    cout << " In loop : " <<  i << endl;
  
    switch(functNameType){ 
      case SUBMITJOB:
        submitJob(sessionKey, machineId, scriptFilePath,  jobInfo);
        std::cout << "jobInfo.getJobId()=" << jobInfo.getJobId() << std::endl;
        break;
      case LISTJOBS:
        listJobs(sessionKey, machineId, listOfJobs);
        std::cout << "listOfJobs.getNbJobs()=" << listOfJobs.getNbJobs() << std::endl;
        break;
      case LISTQUEUES:
        listQueues(sessionKey, machineId, listOfQueues);
        std::cout << "listOfQueues.getNbJobs()=" << listOfQueues.getNbQueues() << std::endl;
        break;
      default:
        std::cerr << "Undefined function to call......." << std::endl;
        break;
    }

    if(functNameType==UNDEFINED) {
      break;
    }
  }
  vishnuFinalize();
  exit(EXIT_SUCCESS);
}


void forker(int cpt,const string& sessionKey, const string& machineId, const FunctionNameType& functNameType){

 // string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");

  cpt--;
  pid_t pid=fork();
  switch(pid){
    case 0 :
      
      //vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
     
      stress(cpt,sessionKey,machineId, functNameType) ;

     // vishnuFinalize();

      break;
    case -1 :
      cout << " Fork number " << cpt << " failed " << endl;
      exit(EXIT_FAILURE);
    default :
      if(cpt>0) {
        cout << "+++++++++++cpt=" << cpt << "++++++++++++++++++" << std::endl;
        forker(cpt,sessionKey,machineId, functNameType);
       }
      wait(NULL);                /* Wait for child */
      break;
  }
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

  if(argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <machineId> <SUBMITJOB|LISTJOBS|LISTQUEUES> " << "[cpt]" << std::endl;
    exit(EXIT_FAILURE); 
  }

  const std::string machineId = argv[1];
  const std::string functionToCallName = argv[2];
  if(functionToCallName!="SUBMITJOB" && functionToCallName!="LISTJOBS" && functionToCallName!="LISTQUEUES") {
    std::cerr << "Unknown name of function  to call .........." << std::endl;
    std::cerr << "Usage: " << argv[0] << " <machineId> <SUBMITJOB|LISTJOBS|LISTQUEUES> <machineId>" << "[cpt]" << std::endl;
    exit(EXIT_FAILURE);
  } 

  const FunctionNameType functNameType = convertToFunctionNameType(functionToCallName);
  
  if (argc > 3){
    cpt = atoi (argv[3]);
  }
  else{
    cpt = 5;
  }

  std::cout << "sess.getSessionKey()=" << sess.getSessionKey() << std::endl;
  forker(cpt, sess.getSessionKey(), machineId, functNameType);

  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);

  close(sess.getSessionKey());

  vishnuFinalize();

  return 0;
}


