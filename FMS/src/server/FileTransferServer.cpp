#include "FileTransferServer.hpp"
#include <vector>
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>
#include "File.hh"
#include "FMSVishnuException.hpp"
#include "utilServer.hpp"
#include <boost/scoped_ptr.hpp>
#include "FileTransferCommand.hpp"
#include <utility>


Database* FileTransferServer::getDatabaseInstance(){

  DbFactory factory;

  return factory.getDatabaseInstance();
}



FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
    const FMS_Data::FileTransfer& fileTransfer,const int& vishnuId):mfileTransfer(fileTransfer),
msessionServer(sessionServer),mvishnuId(vishnuId){

ssh=NULL;
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

ssh=NULL;
}




void FileTransferServer::getUserInfo( std::string& clientMachineName, std::string& userId) {


  std::vector<std::string> result;  
  std::vector<std::string>::const_iterator iter;  
  std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");

  std::string sqlTransferRequest="SELECT name, userid,vsessionid from clmachine,users,vsession where vsession.clmachine_numclmachineid=clmachine.numclmachineid  "
    " and vsession.users_numuserid=users.numuserid and "
    "vsessionid='"+ sessionId+"'";

  boost::scoped_ptr<DatabaseResult> transfer(FileTransferServer::getDatabaseInstance()->getResult(sqlTransferRequest.c_str()));

  if (transfer->getNbTuples()!=0){
    for (size_t i=0; i< transfer->getNbTuples(); ++i){
      result.clear();
      result=transfer->get(i);
      iter=result.begin();
      clientMachineName=*(iter);
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


int FileTransferServer::insertIntoDatabase(int processId){
 

  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert= "insert into fileTransfer (vsession_numsessionid,userId,clientMachineId,sourceMachineId, "
    "destinationMachineId,sourceFilePath,destinationFilePath, transferid,status,fileSize,trCommand,processid,startTime)"
    "values ("+numsession+",'"+mfileTransfer.getUserId()+"','"+ mfileTransfer.getClientMachineId()+"','"+mfileTransfer.getSourceMachineId()+"','"+mfileTransfer.getDestinationMachineId()+"','"
    +mfileTransfer.getSourceFilePath()+"','"+mfileTransfer.getDestinationFilePath() +"','"+mfileTransfer.getTransferId()+"',"+convertToString(mfileTransfer.getStatus())+","
    +convertToString(mfileTransfer.getSize())+","+convertToString(mfileTransfer.getTrCommand())+","+convertToString(processId)+",CURRENT_TIMESTAMP)";

  std::cout << "sqlInsert: " << sqlInsert << "\n";

  FileTransferServer::getDatabaseInstance()->process(sqlInsert);


}

void FileTransferServer::updateData(){

  std::string clientMachineName;
  std::string userId;
  getUserInfo( clientMachineName,  userId);

  std::string vishnuFileTransferId = vishnu::getObjectId(mvishnuId, "filesubcpt", "formatidfiletransfer", FILETRANSFERT,clientMachineName);
  std::cout << "vishnuFileTransferId: " << vishnuFileTransferId << "\n"; 

  mfileTransfer.setClientMachineId(clientMachineName);
  mfileTransfer.setUserId(userId);
  mfileTransfer.setTransferId(vishnuFileTransferId);

}


int FileTransferServer::addTransferThread(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options){

  mfileTransfer.setTrCommand(options.getTrCommand());

  mfileTransfer.setStatus(0); //INPPROGRESS

  updateData();// update datas and get the vishnu transfer id

  if (!file.exists() || false==file.getErrorMsg().empty()) { //if the file does not exist

    mfileTransfer.setStatus(3); //failed
    mfileTransfer.setSize(0); //failed

    mfileTransfer.setStart_time(0);

    insertIntoDatabase();

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,file.getErrorMsg());
  }

  std::cout << "file.getSize(): " << file.getSize()<< "\n";
  mfileTransfer.setSize(file.getSize());
  mfileTransfer.setStart_time(0);
  std::cout << "coucou dans addCpthread avant copy \n";

  // create the thread to perform the copy

  mthread = boost::thread(&FileTransferServer::copy,this, file, dest, options,mfileTransfer.getTransferId());
  boost::posix_time::milliseconds sleepTime(1);

  //FIXME replace by a boost condition variable
  while (ssh==NULL){

    boost::this_thread::sleep(sleepTime);

  } 
  while (ssh->getProcessId()==-1) {

    boost::this_thread::sleep(sleepTime);

  }

  std::cout << "ssh->getProcessId(): " << ssh->getProcessId() << "\n";

  insertIntoDatabase(ssh->getProcessId());

}


void FileTransferServer::copy(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options, const std::string& transferId){


  boost::scoped_ptr<FileTransferCommand> tr ( FileTransferCommand::getCopyCommand(options) );

  std::string trCmd= tr->getCommand();

  ssh=new SSHExec(file.sshCommand, file.scpCommand, file.sshHost, file.sshPort, file.sshUser, file.sshPassword,
      file.sshPublicKey, file.sshPrivateKey);


  std::pair<std::string,std::string> trResult;

  //trResult = ssh.exec(trCmd + " " +getPath()+" "+dest + "\" " + "output.txt");

  trResult = ssh->exec(trCmd + " " +file.getPath()+" "+dest );

  std::cout << "*******trResult=" << trResult.first << std::endl;
 
  if (trResult.second.find("Warning")!=std::string::npos){

    std::cout << "Warning found \n";

    trResult = ssh->exec(trCmd + " "+ file.getPath()+" "+dest);

  }
 
    if (trResult.second.length()!=0) {

      // The file transfer failed
      updateStatus(3,transferId);

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error transfering file: "+trResult.second);

  }
    // The file transfer is  now completed
   
   updateStatus (1,transferId);
}


void FileTransferServer::updateStatus(const FMS_Data::Status& status,const std::string& transferId){


  std::string sqlUpdateRequest = "UPDATE fileTransfer SET status="+convertToString(status)+" where transferid='"+transferId+"'";

  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}


int FileTransferServer::addCpThread(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options){

addTransferThread(file, dest, options);
wait();

}



int FileTransferServer::addCpAsyncThread( const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options){
addTransferThread( file, dest, options);
}




int FileTransferServer::addMvThread(){
  insertIntoDatabase();
}

int FileTransferServer::addMvAsyncThread(){
  insertIntoDatabase();

}



int FileTransferServer::stopThread(const std::string& thrId){}


void FileTransferServer::wait (){

  mthread.join();

}









