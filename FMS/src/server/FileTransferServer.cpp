#include "FileTransferServer.hpp"
#include <vector>
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>

FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
    const FMS_Data::FileTransfer& fileTransfer,const int& vishnuId):mfileTransfer(fileTransfer),
msessionServer(sessionServer),mvishnuId(vishnuId){
DbFactory factory;
mdatabaseVishnu= factory.getDatabaseInstance();

}




    
FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
                   const std::string& srcHost,
                   const std::string& destHost,
                   const std::string& srcFilePath,
                   const std::string& destFilePath,
                   const int& vishnuId):
msessionServer(sessionServer),mvishnuId(vishnuId){

mfileTransfer.setSourceMachineId(srcHost);
mfileTransfer.setDestinationMachineId(destHost);
mfileTransfer.setSourceFilePath(srcFilePath);
mfileTransfer.setDestinationFilePath(destFilePath);


DbFactory factory;
mdatabaseVishnu= factory.getDatabaseInstance();

}




void FileTransferServer::getUserInfo( std::string& name, std::string& userId) {
  std::vector<std::string> result;  
  std::vector<std::string>::const_iterator iter;  
  std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");

  std::string sqlTransferRequest="SELECT name, userid,vsessionid from clmachine,users,vsession where vsession.clmachine_numclmachineid=clmachine.numclmachineid  "
    " and vsession.users_numuserid=users.numuserid and "
    "vsessionid='"+ sessionId+"'";

  boost::scoped_ptr<DatabaseResult> transfer(mdatabaseVishnu->getResult(sqlTransferRequest.c_str()));

  if (transfer->getNbTuples()!=0){
    for (size_t i=0; i< transfer->getNbTuples(); ++i){
      result.clear();
      result=transfer->get(i);
      iter=result.begin();
      name=*(iter);
      userId=*(++iter);

    }
  }

}



const FMS_Data::FileTransfer& FileTransferServer::getFileTransfer() const{

  return mfileTransfer;
}


void FileTransferServer::setFileTransfer( const FMS_Data::FileTransfer& fileTransfer) const{
  mfileTransfer=fileTransfer;
} 


int FileTransferServer::insertIntoDatabase(){
 
  std::string name;
  std::string userId;
  getUserInfo( name,  userId);


  std::string vishnuFileTransferId = vishnu::getObjectId(mvishnuId, "filesubcpt", "formatidfiletransfer", FILETRANSFERT,name);

  mfileTransfer.setClientMachineId(name);
  mfileTransfer.setUserId(userId);
  mfileTransfer.setTransferId(vishnuFileTransferId);

int status =0; // TODO A calculer
int fileSize =10; // TODO A calculer
int trCommand =1; // TODO A calculer
int processId =2011; // TODO A calculer

  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert= "insert into fileTransfer (vsession_numsessionid,userId,clientMachineId,sourceMachineId, "
    "destinationMachineId,sourceFilePath,destinationFilePath, transferid,status,fileSize,trCommand,processid,startTime)"
    "values ("+numsession+",'"+userId+"','"+ name+"','"+mfileTransfer.getSourceMachineId()+"','"+mfileTransfer.getDestinationMachineId()+"','"
    +mfileTransfer.getSourceFilePath()+"','"+mfileTransfer.getDestinationFilePath() +"','"+vishnuFileTransferId+"',"+convertToString(status)+","
    +convertToString(fileSize)+","+convertToString(trCommand)+","+convertToString(processId)+",CURRENT_TIMESTAMP)";

mdatabaseVishnu->process(sqlInsert);


}


int FileTransferServer::addCpThread(){
  insertIntoDatabase();

}

int FileTransferServer::addMvThread(){
  insertIntoDatabase();

}

int FileTransferServer::addCpAsyncThread(){
  insertIntoDatabase();

}
int FileTransferServer::addMvAsyncThread(){
  insertIntoDatabase();

}



int FileTransferServer::stopThread(const std::string& thrId){}


void FileTransferServer::wait (){

  mthread.join();

}

