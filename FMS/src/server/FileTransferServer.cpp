
#include "FileFactory.hpp"
#include "FileTransferServer.hpp"
#include <vector>
#include <cstring>
#include <list>
#include "utilVishnu.hpp"
#include <boost/scoped_ptr.hpp>
#include "File.hpp"
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
#include "ListFileTransfers.hpp"
#include "OptionValueServer.hpp"

using namespace std;

namespace ba=boost::algorithm;

unsigned int FileTransferServer::msshPort=22;
std::string FileTransferServer::msshCommand="/usr/bin/ssh";

// Get The Database instance
Database* FileTransferServer::getDatabaseInstance(){

  Database* databaseInstance (DbFactory().getDatabaseInstance());
  if(databaseInstance ==NULL){
    throw SystemException(ERRCODE_DBERR, "Get a null instance of database" );
  }
    return databaseInstance;
}

// Check The transfer id

void FileTransferServer::checkTransferId(std::string transferId) {
  std::string sqlTransferRequest = "SELECT transferId from filetransfer where transferId='"+transferId+"'";
  boost::scoped_ptr<DatabaseResult> transfer(FileTransferServer::getDatabaseInstance()->getResult(sqlTransferRequest.c_str()));
  if(transfer->getNbTuples()==0) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
  }
}

// add a query to the sql request

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

// A constructor with parameters

FileTransferServer::FileTransferServer(const SessionServer& sessionServer,const int& vishnuId):
  msessionServer(sessionServer),mvishnuId(vishnuId),mtransferType(File::undefined){

  }

// Another constructor with parameters

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


// To get a user info from the database

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

// To get the file transfer information

const FMS_Data::FileTransfer& FileTransferServer::getFileTransfer() const{

  return mfileTransfer;
}

// To update the file transfer information

void FileTransferServer::setFileTransfer( const FMS_Data::FileTransfer& fileTransfer) const{
  mfileTransfer=fileTransfer;
} 

// To log data into database 

int FileTransferServer::logIntoDatabase(int processId, const std::string& errorMsg){

  std::string errorMsgCleaned=FileTransferServer::filterString(errorMsg);

  std::string numsession = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "numsessionid");
  std::string sqlInsert= "insert into filetransfer (vsession_numsessionid,userId,clientMachineId,sourceMachineId, "
    "destinationMachineId,sourceFilePath,destinationFilePath, transferid,status,fileSize,trCommand,processid,errorMsg,startTime)"
    "values ("+numsession+",'"+mfileTransfer.getUserId()+"','"+ mfileTransfer.getClientMachineId()+"','"+mfileTransfer.getSourceMachineId()+"','"+mfileTransfer.getDestinationMachineId()+"','"
    +mfileTransfer.getSourceFilePath()+"','"+mfileTransfer.getDestinationFilePath() +"','"+mfileTransfer.getTransferId()+"',"+convertToString(mfileTransfer.getStatus())+","
    +convertToString(mfileTransfer.getSize())+","+convertToString(mfileTransfer.getTrCommand())+","+convertToString(processId)+",'"+errorMsgCleaned+"'" +",CURRENT_TIMESTAMP)";


  FileTransferServer::getDatabaseInstance()->process(sqlInsert);


}

// update file transfer data

void FileTransferServer::updateData(){

  std::string clientMachineName;
  std::string userId;
  getUserInfo( clientMachineName,  userId);

  std::string vishnuFileTransferId = vishnu::getObjectId(mvishnuId, "filesubcpt", "formatidfiletransfer", FILETRANSFERT,clientMachineName);

  mfileTransfer.setClientMachineId(clientMachineName);
  mfileTransfer.setUserId(userId);
  mfileTransfer.setTransferId(vishnuFileTransferId);

}

// To add a new file transfer thread

int FileTransferServer::addTransferThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){


  FMS_Data::CpFileOptions newOptions (options);

  if (options.getTrCommand()==2){

    std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");

    std::string sqlCommand="SELECT users.numuserid,users_numuserid,vsessionid from users,vsession where vsession.users_numuserid=users.numuserid and "
      "vsessionid='"+ sessionId+"'";

    boost::scoped_ptr<DatabaseResult> dbResult(FileTransferServer::getDatabaseInstance()->getResult(sqlCommand.c_str()));

    if(dbResult->getNbTuples()!=0){
      std::string numuserId= dbResult->getFirstElement(); 
      OptionValueServer optionValueServer;
      newOptions.setTrCommand(optionValueServer.getOptionValueForUser
(numuserId, TRANSFERCMD_OPT));
    }

  }

  boost::scoped_ptr<FileTransferCommand> tr ( FileTransferCommand::getCopyCommand(msessionServer,newOptions) );

  std::string trCmd= tr->getCommand();


  boost::scoped_ptr<SSHFile> srcFileServer (new SSHFile(msessionServer, mfileTransfer.getSourceFilePath(),srcMachineName, srcUser, "", srcUserKey, "", FileTransferServer::getSSHPort(), FileTransferServer::getSSHCommand(), tr->getLocation()));


  mfileTransfer.setTrCommand(newOptions.getTrCommand());

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


  if ( (srcUser==destUser) &&  (srcMachineName== destMachineName)  && (mfileTransfer.getSourceFilePath()==mfileTransfer.getDestinationFilePath())     ){

    mfileTransfer.setStatus(3); //failed
    logIntoDatabase(-1,"same source and destination ");

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"same source and destination ");

  }

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



// To perform a file copy



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

  // Clean the output message 
  std::string allOutputMsg (FileTransferServer::cleanOutputMsg(trResult.first+trResult.second));


  if (allOutputMsg.length()!=0 ) {

    // The file transfer failed
    updateStatus(3,transferExec.getTransferId(),allOutputMsg);

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

// To update file transfer status into database

void FileTransferServer::updateStatus(const FMS_Data::Status& status,const std::string& transferId, const std::string& errorMsg){

  std::string errorMsgCleaned=FileTransferServer::filterString(errorMsg);

  std::string sqlUpdateRequest = "UPDATE filetransfer SET status="+convertToString(status)+", errorMsg='"+errorMsgCleaned+"'"+ " where transferid='"+transferId+"'"
    + " and status<>2";
  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}

// get error message from database

string FileTransferServer::getErrorFromDatabase(const std::string& transferid){

  std::string sqlCommand = "SELECT errormsg from filetransfer where transferid='"+ transferid +"'";

  boost::scoped_ptr<DatabaseResult> result(FileTransferServer::getDatabaseInstance()->getResult(sqlCommand.c_str()));

  return result->getFirstElement();


}

/**
 * \brief Clean an output message
 * remove the line beginning by "Pseudo-terminal"
 */

std::string  FileTransferServer::cleanOutputMsg(const std::string& allOutputMsg){

  if (ba::starts_with(allOutputMsg,"Pseudo-terminal") ) {

    size_t pos =allOutputMsg.find_first_of("\n");

    if (pos!=std::string::npos){

      return filterString(allOutputMsg.substr(pos+1));

    }


  }
  return filterString(allOutputMsg);
}


// A file copy thread


int FileTransferServer::addCpThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){
  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
  waitThread();

  std:: string errorMsg(getErrorFromDatabase(mfileTransfer.getTransferId()));

  if(false==errorMsg.empty()){
    throw FMSVishnuException (ERRCODE_RUNTIME_ERROR,errorMsg);

  }
  return 0;

}


// add a new synchronous file copy thread

int FileTransferServer::addCpAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);

}


// add a new synchronous move file  thread

int FileTransferServer::addMvThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);
  waitThread();

  std:: string errorMsg(getErrorFromDatabase(mfileTransfer.getTransferId()));

  if(false==errorMsg.empty()){
    throw FMSVishnuException (ERRCODE_RUNTIME_ERROR,errorMsg);

  }
  return 0;

}

// add a new asynchronous file move thread


int FileTransferServer::addMvAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);

}

// Wait until a thread terminates

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

  std::string transferId(options.getTransferId()),machineName(options.getFromMachineId()),userId(options.getUserId());

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
  if (machineName.size() != 0) {
    
    sqlRequest.append(" and (sourceMachineId='"+machineName+"'"+" or destinationMachineId='"+machineName+"')"); 
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

// Cancel  file transfers 

int FileTransferServer::stopThread(const FMS_Data::StopTransferOptions& options ){


  if (false==options.getTransferId().empty() || false==options.getUserId().empty() || false==options.getFromMachineId().empty()){


    std::string sqlListOfPid = "SELECT transferId,processId from filetransfer, vsession "
      "where vsession.numsessionid=filetransfer.vsession_numsessionid and filetransfer.status=0";

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
        stopThread(transferid,pid);
        ++iter;
      }

    }
    else {
      throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "There is no file transfer in progress ");
    }
  }

  return 0;
}



// cancel a file transfer 


int FileTransferServer::stopThread(const std::string& transferid,const int& pid ){

  int result=0;

if (pid!=-1){

  if(result=kill(pid, SIGTERM)){

    updateStatus(3,transferid,strerror(errno));
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,strerror(errno));
  }

  // get the user responsible for the stop request

  std::string sessionId = msessionServer.getAttribut("where sessionkey='"+(msessionServer.getData()).getSessionKey()+"'", "vsessionid");

  std::string sqlCommand="SELECT userid,vsessionid from users,vsession where vsession.users_numuserid=users.numuserid and "
    "vsessionid='"+ sessionId+"'";

  boost::scoped_ptr<DatabaseResult> dbResult(FileTransferServer::getDatabaseInstance()->getResult(sqlCommand.c_str()));

  std::string logMsg= "by: "+ dbResult->getFirstElement();

  // log into database

  updateStatus(2,transferid,logMsg);
}
  return result;
}


// update an ssh port

void FileTransferServer::setSSHPort(const unsigned int sshPort){
  msshPort=sshPort;
}


// update the ssh command path

void FileTransferServer::setSSHCommand(const std::string& sshCommand){
  msshCommand=sshCommand;
}

// get the ssh port
const unsigned int FileTransferServer::getSSHPort(){
  return msshPort;
} 
const std::string& FileTransferServer::getSSHCommand( ){
  return msshCommand;
}

// filter string by removing some character like :, ', "

std::string FileTransferServer::filterString(  const std::string& toFilter){

  std::string cleanString=ba::erase_all_copy( ba::erase_all_copy( ba::erase_all_copy(ba::erase_all_copy(toFilter,":"),"'"),"`"),"\"");

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

// get a session server object

const SessionServer& TransferExec::getSessionServer() const{
  return msessionServer;
}

// get the source machine user object
const std::string& TransferExec::getSrcUser() const{
  return msrcUser;
}

// get the source machine name
const std::string& TransferExec::getSrcMachineName() const{
  return msrcMachineName;
}


// get the source file path

const std::string& TransferExec::getSrcPath() const{
  return  msrcPath;
}


// get the source machine user ssh private key path
const std::string& TransferExec::getSrcUserKey() const{
  return msrcUserKey;
}


// get the destination machine user object

const std::string& TransferExec::getDestUser() const{
  return mdestUser;
}

// get the destination machine name

const std::string& TransferExec::getDestMachineName() const{
  return  mdestMachineName;
}

// get the destination file path

const std::string& TransferExec::getDestPath() const{
  return mdestPath;
}

// get the file transfer identifier

const std::string& TransferExec::getTransferId() const{
  return mtransferId;
}

// get the file transfer process identifier

const int& TransferExec::getProcessId() const{
  return mprocessId;
}

// set the file transfer process identifier
void TransferExec::setProcessId(const int&  processId) const{
  mprocessId=processId;
}

// get the file transfer process  last execution status

const int& TransferExec::getLastExecStatus() const{
  return mlastExecStatus;
}

// Set the file transfer process  last execution status

void TransferExec::setLastExecStatus(const int& lastExecStatus) const{

  mlastExecStatus=lastExecStatus;
}


// Set the file transfer process identifier into database

void TransferExec::updatePid(const int& pid)const {

  setProcessId(pid);
  std::string sqlUpdateRequest = "UPDATE filetransfer SET processid="+convertToString(pid)+" where transferId='"+getTransferId()+"'";
  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}

// Perform a remote command through ssh



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
  command <<" -C "  << " -o BatchMode=yes " << " -o StrictHostKeyChecking=no";
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

  return result;

}




