#include "UMS_Data_forward.hpp"
#include "FMS_Data_forward.hpp"
#include <unistd.h>
#include <iostream>
#include "api_ums.hpp"
#include "api_fms.hpp"
#include "FMS_testconfig.h"
#include "diet_config_tests.h"
using namespace std;
using namespace UMS_Data;
using namespace vishnu;

void stress(int cpt,const string& sessionKey,const string& machineId,int type){

  string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
  ostringstream fileName;
  fileName <<machineId;
  fileName <<":/tmp/test-";
  fileName <<cpt;
  fileName << "-";
  vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);

  if (type == 1) { // list file transfers
  
  FMS_Data::LsTransferOptions lsFileTransferOptions;
  lsFileTransferOptions.setStatus(1);
    FMS_Data::FileTransferList fileTransferList;
    
    for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;
    listFileTransfers(sessionKey,fileTransferList,lsFileTransferOptions);
    }
    std::cout << "5 iterations done \n";
  }
  else if (type ==2){ // create an remove file
  
    for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;

      fileName <<i;
      // create adn remove file

      std::string path(fileName.str());
      createFile(sessionKey, path);
     removeFile(sessionKey, path);

    }
    std::cout << "5 iterations done \n";

  } else if (type ==3){ //copy
    
      for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;

      fileName <<i;
      // create adn remove file

      std::string path(fileName.str());
      copyFile(sessionKey,dietClientConfigPath,path);

    }
    std::cout << "5 iterations done \n";

  
  }
  else if (type ==4){// getFileInfo
  
    FileStat fileInfos; 
      for (int i=0;i<5;i++) {
      cout << " In loop : " <<  i << endl;
      getFileInfo(sessionKey,machineId+":/proc",fileInfos);

    }
    std::cout << "5 iterations done \n";

  
  }
  vishnuFinalize();
}


void forker(int cpt,const string& sessionKey,const std::string& machineId, int type){
  cpt--;

  switch(fork()){
    case 0 :
      stress( cpt,sessionKey,machineId,type) ;
      break;
    case -1 :
      cout << " Fork number " << cpt << " failed " << endl;
      break;
    default :
      if(cpt>0)
        forker(cpt,sessionKey,machineId,type);
      break;
  }
  return;
}


int main(int argc, char** argv){
  int cpt;
  int i;
  int type;
  string key  = ""       ;
  string pwd  = "toto"  ;
  string uid  = "user_1";
  string machineId  = "MA_2";
  ConnectOptions cop  ;
  Session sess ;

  string dietClientConfigPath =CONFIG_DIR + string("/client_testing.cfg");
  cop.setClosePolicy(2);

  try{
    vishnuInitialize((char*) dietClientConfigPath.c_str(), 0, NULL);
    connect    (uid, pwd, sess, cop );
    vishnuFinalize();

    if (argc != 3) {
      std::cerr << "Usage: ./fms_stress Nbtests Type "<< std::endl;
      std::cerr << "[1 for listfiletransfer and 2 for createFile]" << std::endl;
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
    if ((type != 1) && (type != 2) && (type != 3) && (type != 4)) {
       std::cerr << "The value of the type is incorrect" << std::endl;
       return 1;
    }
    std::cout << "sess.getSessionKey()=" << sess.getSessionKey() << std::endl;
    forker(cpt, sess.getSessionKey(),machineId,type);
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


