
#include "FileFactory.hh"
#include "FileTransferServer.hpp"
#include <vector>
#include <cstring>
#include <list>
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>
#include "File.hh"
#include "FMSVishnuException.hpp"
#include "utilServer.hpp"
#include <boost/scoped_ptr.hpp>
#include "FileTransferCommand.hpp"
#include <utility>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "ListFileTransfers.hh"

using namespace std;

namespace ba=boost::algorithm;
unsigned int FileTransferServer::msshPort=22;
std::string FileTransferServer::msshCommand="/usr/bin/ssh";


Database* FileTransferServer::getDatabaseInstance(){

  DbFactory factory;

  return factory.getDatabaseInstance();
}

void FileTransferServer::checkTransferId(std::string transferId) {
  std::string sqlTransferRequest = "SELECT transferId from filetransfer where transferId='"+transferId+"'";
  boost::scoped_ptr<DatabaseResult> transfer(FileTransferServer::getDatabaseInstance()->getResult(sqlTransferRequest.c_str()));
  if(transfer->getNbTuples()==0) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
  }
}

void FileTransferServer::addOptionRequest(const std::string& name, const std::string& value, std::string& request) {
  request.append(" and "+name+"=");
  request.append("'"+value+"'");
}


/**
 * \brief Function to check if a given user identifier exists
 * \fn void checkUserId(std::string userId)
 * \param userId the user identifier
 * \return raises an exception on error
 */
void FileTransferServer::checkUserId(std::string userId) {
  std::string sqlUserRequest = "SELECT userid from users where userid='"+userId+"'";
  boost::scoped_ptr<DatabaseResult> user(FileTransferServer::getDatabaseInstance()->getResult(sqlUserRequest.c_str()));
  if(user->getNbTuples()==0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
  }
}


/**
 * \brief Function to check if a given machine client identifier exists
 * \fn void checkClientMachineName(std::string clmachineId)
 * \param clmachineId the machine client identifier
 */
void FileTransferServer::checkClientMachineName(std::string clmachineId) {
  std::string sqlclMachineRequest = "SELECT name from clmachine where name='"+clmachineId+"'";
  boost::scoped_ptr<DatabaseResult> clmachine(FileTransferServer::getDatabaseInstance()->getResult(sqlclMachineRequest.c_str()));
  if(clmachine->getNbTuples()==0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE);
  }
}



FileTransferServer::FileTransferServer(const SessionServer& sessionServer,const int& vishnuId):
  msessionServer(sessionServer),mvishnuId(vishnuId),mtransferType(File::undefined){

  }


FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
    const std::string& srcHost,
    const std::string& destHost,
    const std::string& srcFilePath,
    const std::string& destFilePath,
    const int& vishnuId):
  msessionServer(sessionServer),mvishnuId(vishnuId),mtransferType(File::undefined){

    mfileTransfer.setSourceMachineId(srcHost);
    mfileTransfer.setDestinationMachineId(destHost);
    mfileTransfer.setSourceFilePath(srcFilePath);
    mfileTransfer.setDestinationFilePath(destFilePath);

  }




void FileTransferServer::getUserInfo( std::string& clientMachineName, std::string& userId) {


  std::string home;
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


int FileTransferServer::logIntoDatabase(int processId, const std::string& errorMsg){

  std::string errorMsgCleaned=FileTransferServer::filterString(errorMsg);

  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert= "insert into fileTransfer (vsession_numsessionid,userId,clientMachineId,sourceMachineId, "
    "destinationMachineId,sourceFilePath,destinationFilePath, transferid,status,fileSize,trCommand,processid,errorMsg,startTime)"
    "values ("+numsession+",'"+mfileTransfer.getUserId()+"','"+ mfileTransfer.getClientMachineId()+"','"+mfileTransfer.getSourceMachineId()+"','"+mfileTransfer.getDestinationMachineId()+"','"
    +mfileTransfer.getSourceFilePath()+"','"+mfileTransfer.getDestinationFilePath() +"','"+mfileTransfer.getTransferId()+"',"+convertToString(mfileTransfer.getStatus())+","
    +convertToString(mfileTransfer.getSize())+","+convertToString(mfileTransfer.getTrCommand())+","+convertToString(processId)+",'"+errorMsgCleaned+"'" +",CURRENT_TIMESTAMP)";


  FileTransferServer::getDatabaseInstance()->process(sqlInsert);


}



void FileTransferServer::updateData(){

  std::string clientMachineName;
  std::string userId;
  getUserInfo( clientMachineName,  userId);

  std::string vishnuFileTransferId = vishnu::getObjectId(mvishnuId, "filesubcpt", "formatidfiletransfer", FILETRANSFERT,clientMachineName);

  mfileTransfer.setClientMachineId(clientMachineName);
  mfileTransfer.setUserId(userId);
  mfileTransfer.setTransferId(vishnuFileTransferId);

}


int FileTransferServer::addTransferThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  boost::scoped_ptr<FileTransferCommand> tr ( FileTransferCommand::getCopyCommand(options) );

  std::string trCmd= tr->getCommand();


  boost::scoped_ptr<SSHFile> srcFileServer (new SSHFile(msessionServer, mfileTransfer.getSourceFilePath(),srcMachineName, srcUser, "", srcUserKey, "", FileTransferServer::getSSHPort(), FileTransferServer::getSSHCommand(), tr->getLocation()));



  mfileTransfer.setTrCommand(options.getTrCommand());

  mfileTransfer.setStatus(0); //INPPROGRESS

  updateData();// update datas and get the vishnu transfer id

  if (!srcFileServer->exists() || false==srcFileServer->getErrorMsg().empty()) { //if the file does not exist

    mfileTransfer.setStatus(3); //failed
    mfileTransfer.setSize(0); //failed

    mfileTransfer.setStart_time(0);

    logIntoDatabase(-1,srcFileServer->getErrorMsg());

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,srcFileServer->getErrorMsg());
  }

  mfileTransfer.setSize(srcFileServer->getSize());
  mfileTransfer.setStart_time(0);

  // create a TransferExec instance

  TransferExec transferExec (msessionServer,srcUser,srcMachineName,mfileTransfer.getSourceFilePath(), srcUserKey, destUser,destMachineName, mfileTransfer.getDestinationFilePath(),mfileTransfer.getTransferId());


  logIntoDatabase(-1);

  // create the thread to perform the copy

  if(mtransferType==File::copy){
    mthread = boost::thread(&FileTransferServer::copy,this, transferExec,trCmd);
  }else if (mtransferType==File::move){
    mthread = boost::thread(&FileTransferServer::move,this, transferExec,trCmd);
  }



}


void FileTransferServer::copy(const TransferExec& transferExec, const std::string& trCmd)

{

  //build the destination complete path

  std::ostringstream destCompletePath;
  destCompletePath << transferExec.getDestUser() << "@"<< transferExec.getDestMachineName() <<":"<<transferExec.getDestPath();


  std::pair<std::string,std::string> trResult;


  trResult = transferExec.exec(trCmd + " " +transferExec.getSrcPath()+" "+destCompletePath.str() );

  if (trResult.second.find("Warning")!=std::string::npos || trResult.first.find("Warning")!=std::string::npos ){

    trResult = transferExec.exec(trCmd + " " +transferExec.getSrcPath()+" "+destCompletePath.str() );

  }

  if (trResult.second.length()!=0 || trResult.first.length()!=0) {

    // The file transfer failed
    updateStatus(3,transferExec.getTransferId(),trResult.second+trResult.first);

   // throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error transfering file: "+trResult.second);

  }else{
   
    // The file transfer is  now completed
    
    updateStatus (1,transferExec.getTransferId(),"");

  }
}


void FileTransferServer::move(const TransferExec& transferExec, const std::string& trCmd){
  
  // perform the copy
  copy(transferExec,trCmd);
  int lastExecStatus=transferExec.getLastExecStatus();

  if (lastExecStatus==0){

    // remove the source file
    FileFactory::setSSHServer(transferExec.getSrcMachineName());
    boost::scoped_ptr<File> file (FileFactory::getFileServer( transferExec.getSessionServer(),transferExec.getSrcPath(),
          transferExec.getSrcUser(),transferExec.getSrcUserKey() ) ) ;
    try{
      file->rm(true);
    }catch(VishnuException& err){
      updateStatus(3,transferExec.getTransferId(),err.what());
      transferExec.setLastExecStatus(1);

    }

  }
}




void FileTransferServer::updateStatus(const FMS_Data::Status& status,const std::string& transferId, const std::string& errorMsg){

  std::string errorMsgCleaned=FileTransferServer::filterString(errorMsg);

  std::string sqlUpdateRequest = "UPDATE filetransfer SET status="+convertToString(status)+", errorMsg='"+errorMsgCleaned+"'"+ " where transferid='"+transferId+"'"
                                  + " and status<>2";
  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}


int FileTransferServer::addCpThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){
  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
  waitThread();

}

int FileTransferServer::addCpAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);

}


int FileTransferServer::addMvThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);
  waitThread();
}

int FileTransferServer::addMvAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);

}


void FileTransferServer::waitThread (){

  mthread.join();
}


/**
 * \brief Function to treat the Stop file transfer options
 * \param options the object which contains the Stop file transfers options values
 * \param sqlRequest the sql data base request
 * \return raises an exception on error
 */
void
FileTransferServer::processOptions(const FMS_Data::StopTransferOptions& options, std::string& sqlRequest){
  
  std::string transferId(options.getTransferId()),clientMachineName(options.getFromMachineId()),userId(options.getUserId());

  //To check if the transferId is defined
  if (transferId.size() != 0) {

    if(transferId.compare("all")!=0 && transferId.compare("ALL")!=0){
    //To check the transfer Id
    FileTransferServer::checkTransferId(options.getTransferId());
    //To add the transferId on the request
    FileTransferServer::addOptionRequest("transferId", options.getTransferId(), sqlRequest);
 
    }
   
  }

  //To check if the fromMachineId is defined
  if (clientMachineName.size() != 0) {
    //To check the client machine Id
    FileTransferServer::checkClientMachineName(options.getFromMachineId());
    //To add the fromMachineId on the request
    FileTransferServer::addOptionRequest("clientMachineId", options.getFromMachineId(), sqlRequest);
  }

  //Creation of the object user
    UserServer userServer = UserServer(msessionServer);

    userServer.init();

    //To check if the userId is defined
    if (userId.size() != 0) {

      if (!userServer.isAdmin()) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
      }

      if(userId.compare("all")!=0 && userId.compare("ALL")!=0){
    //To check the user Id
    FileTransferServer::checkUserId(options.getUserId());

    //To add the userId on the request
    FileTransferServer::addOptionRequest("userId", userId, sqlRequest);

    }
  }else{
  
   FileTransferServer::addOptionRequest("userId", userServer.getData().getUserId(), sqlRequest);
  
  }

}



int FileTransferServer::stopThread(const FMS_Data::StopTransferOptions& options ){


  if (false==options.getTransferId().empty() || false==options.getUserId().empty() || false==options.getFromMachineId().empty()){

  
    std::string sqlListOfPid = "SELECT transferid,processId from filetransfer, vsession "
    "where vsession.numsessionid=fileTransfer.vsession_numsessionid and filetransfer.status=0";
    
    std::string transferid;
    int pid;
  std::vector<std::string>::iterator iter;
  std::vector<std::string> results;

  msessionServer.check();

  processOptions(options, sqlListOfPid);

  boost::scoped_ptr<DatabaseResult> ListOfPid (FileTransferServer::getDatabaseInstance()->getResult(sqlListOfPid.c_str()));

  if (ListOfPid->getNbTuples() != 0){
    for (size_t i = 0; i < ListOfPid->getNbTuples(); ++i) {
      results.clear();
      results = ListOfPid->get(i);
      iter = results.begin();
      transferid=*iter;
      ++iter;
      pid=convertToInt(*iter);
      stopThread(transferid,pid );
      ++iter;
    }

  }
  else {
  throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "There is no file transfer in progress ");
  }
  }

  return 0;
}
int FileTransferServer::stopThread(const std::string& transferid,const int& pid ){

  int result;

  if(result=kill(pid, SIGTERM)){

    updateStatus(3,transferid,strerror(errno));
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,strerror(errno));
  }
    
  updateStatus(2,transferid,"");
  return result;
}


void FileTransferServer::setSSHPort(const unsigned int sshPort){
  msshPort=sshPort;
}

void FileTransferServer::setSSHCommand(const std::string& sshCommand){
  msshCommand=sshCommand;
}

const unsigned int FileTransferServer::getSSHPort(){
  return msshPort;
} 
const std::string& FileTransferServer::getSSHCommand( ){
  return msshCommand;
}


std::string FileTransferServer::filterString(  const std::string& toFilter){

  std::string cleanString=ba::erase_all_copy( ba::erase_all_copy(ba::erase_all_copy(toFilter,":"),"'"),"`");

  return cleanString; 

} 


// TransferExec Class

TransferExec::TransferExec (const SessionServer& sessionServer,
    const std::string& srcUser,
    const std::string& srcMachineName,
    const std::string& srcPath,
    const std::string& srcUserKey,
    const std::string& destUser,
    const std::string& destMachineName,
    const std::string& destPath,
    const std::string& transferId):msessionServer(sessionServer),
  msrcUser(srcUser),
  msrcMachineName(srcMachineName),
  msrcPath(srcPath),
  msrcUserKey(srcUserKey),
  mdestUser(destUser),
  mdestMachineName(destMachineName),
  mdestPath(destPath),                            
  mtransferId(transferId){

    mprocessId=-1;
    mlastExecStatus=0;
  }


const SessionServer& TransferExec::getSessionServer() const{
  return msessionServer;
}


const std::string& TransferExec::getSrcUser() const{
  return msrcUser;
}

const std::string& TransferExec::getSrcMachineName() const{
  return msrcMachineName;
}

const std::string& TransferExec::getSrcPath() const{
  return  msrcPath;
}

const std::string& TransferExec::getSrcUserKey() const{
  return msrcUserKey;
}

const std::string& TransferExec::getDestUser() const{
  return mdestUser;
}

const std::string& TransferExec::getDestMachineName() const{
  return  mdestMachineName;
}

const std::string& TransferExec::getDestPath() const{
  return mdestPath;
}

const std::string& TransferExec::getTransferId() const{
  return mtransferId;
}


const int& TransferExec::getProcessId() const{
  return mprocessId;
}

void TransferExec::setProcessId(const int&  processId) const{
  mprocessId=processId;
}
const int& TransferExec::getLastExecStatus() const{
  return mlastExecStatus;
}

void TransferExec::setLastExecStatus(const int& lastExecStatus) const{

  mlastExecStatus=lastExecStatus;
}

void TransferExec::updatePid(const int& pid)const {

  setProcessId(pid);
  std::string sqlUpdateRequest = "UPDATE fileTransfer SET processid="+convertToString(pid)+" where transferid='"+getTransferId()+"'";
  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}

std::pair<std::string, std::string> TransferExec::exec(const std::string& cmd) const{
  vector<string> tokens;
  ostringstream command;
  pid_t pid;
  pair<string,string> result;
  int comPipeOut[2];
  int comPipeErr[2];
  int status;
  char c;

  command  << FileTransferServer::getSSHCommand() << " -t -q " << " -l " << getSrcUser();
  command <<" -C"  << " -o BatchMode=yes " << " -o StrictHostKeyChecking=no";
  command << " -o ForwardAgent=yes";
  command << " -p " << FileTransferServer::getSSHPort() << " " << getSrcMachineName() << " " << cmd;


  istringstream is(command.str());

  copy(istream_iterator<string>(is),
      istream_iterator<string>(),
      back_inserter<vector<string> >(tokens));

  /***********************************************/

  
  char* argv[tokens.size()+1];
  argv[tokens.size()]=NULL;

  for (unsigned int i=0; i<tokens.size(); ++i)
    argv[i]=strdup(tokens[i].c_str());

  if (pipe(comPipeOut)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating communication pipe");
  }
  if (pipe(comPipeErr)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    close(comPipeOut[0]);
    close(comPipeOut[1]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating communication pipe");
  }
  pid = fork();

  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error forking process");
  }
  if (pid==0) {
    close(comPipeOut[0]); /* Close unused read end */
    close(comPipeErr[0]); /* Close unused write end */
    dup2(comPipeOut[1], 1);
    dup2(comPipeErr[1], 2);
    close(comPipeOut[1]);
    close(comPipeErr[1]);
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }

  // Store the child process id
  updatePid (pid);

  close(comPipeOut[1]); /* Close unused write end */

  close(comPipeErr[1]);/* Close unused write end */


  while (read(comPipeOut[0], &c, 1))
    result.first+=c;

  while (read(comPipeErr[0], &c, 1))
    result.second+=c;

  if (waitpid(pid, &status, 0)==-1) {
    close(comPipeOut[0]);
    close(comPipeErr[0]);
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error executing command "+command.str());
  }

  close(comPipeOut[0]);
  close(comPipeErr[0]);
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  mlastExecStatus = status;
  std::cout << "result.second: " << result.second << "\n";
  return result;

}





