
#include "FileFactory.hh"
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
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

using namespace std;


unsigned int FileTransferServer::msshPort=22;
std::string FileTransferServer::msshCommand="/usr/bin/ssh";


Database* FileTransferServer::getDatabaseInstance(){

  DbFactory factory;

  return factory.getDatabaseInstance();
}



FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
    const FMS_Data::FileTransfer& fileTransfer,const int& vishnuId):mfileTransfer(fileTransfer),
msessionServer(sessionServer),mvishnuId(vishnuId){

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

    insertIntoDatabase();

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,srcFileServer->getErrorMsg());
  }

  std::cout << "srcFileServer->getSize(): " << srcFileServer->getSize()<< "\n";
  mfileTransfer.setSize(srcFileServer->getSize());
  mfileTransfer.setStart_time(0);
  std::cout << "coucou dans addCpthread avant copy \n";

// create a TransferExec instance

TransferExec transferExec(srcUser,srcMachineName,mfileTransfer.getSourceFilePath(), srcUserKey, destUser,destMachineName, mfileTransfer.getDestinationFilePath(),mfileTransfer.getTransferId());


  // create the thread to perform the copy

  mthread = boost::thread(&FileTransferServer::copy,this, transferExec,trCmd);
  boost::posix_time::milliseconds sleepTime(1);

  //FIXME replace by a boost condition variable
  while (transferExec.getProcessId()==-1) {

    boost::this_thread::sleep(sleepTime);

  }

  
  std::cout << "transferExec.getProcessId(): " << transferExec.getProcessId() << "\n";

  insertIntoDatabase(transferExec.getProcessId());

}


void FileTransferServer::copy(const TransferExec& transferExec, const std::string& trCmd)

{


  //build the source and destination complete path
  std::ostringstream srcCompletePath;
  srcCompletePath << transferExec. getSrcUser() << "@"<<transferExec. getSrcMachineName() <<":"<<transferExec.getSrcPath();
  std::cout << "srcCompletePath " <<srcCompletePath.str() << "\n";

  std::ostringstream destCompletePath;
  destCompletePath << transferExec.getDestUser() << "@"<< transferExec.getDestMachineName() <<":"<<transferExec.getDestPath();

  std::cout << "destCompletePath " <<destCompletePath.str() << "\n";


  std::pair<std::string,std::string> trResult;

  //trResult = ssh.exec(trCmd + " " +getPath()+" "+dest + "\" " + "output.txt");

  trResult = transferExec.exec(trCmd + " " +transferExec.getSrcPath()+" "+destCompletePath.str() );

  std::cout << "*******trResult=" << trResult.first << std::endl;
 
  if (trResult.second.find("Warning")!=std::string::npos){

    std::cerr << "Warning found \n";

  trResult = transferExec.exec(trCmd + " " +srcCompletePath.str()+" "+destCompletePath.str() );

  }
 
    if (trResult.second.length()!=0) {

      // The file transfer failed
      updateStatus(3,transferExec.getTransferId());

    //throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error transfering file: "+trResult.second);

  }else{
    // The file transfer is  now completed
   
   updateStatus (1,transferExec.getTransferId());
  }
}


void FileTransferServer::updateStatus(const FMS_Data::Status& status,const std::string& transferId){


  std::string sqlUpdateRequest = "UPDATE fileTransfer SET status="+convertToString(status)+" where transferid='"+transferId+"'";

  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest.c_str());

}


int FileTransferServer::addCpThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
waitThread();

}

int FileTransferServer::addCpAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options){

addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);

}





int FileTransferServer::addMvThread(){
  insertIntoDatabase();
}

int FileTransferServer::addMvAsyncThread(){
  insertIntoDatabase();

}


void FileTransferServer::waitThread (){

  mthread.join();
}






int FileTransferServer::stopThread(const std::string& thrId){}




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
 











// TransferExec Class

 TransferExec::TransferExec (const std::string& srcUser,
                             const std::string& srcMachineName,
                             const std::string& srcPath,
                             const std::string& srcUserKey,
                             const std::string& destUser,
                             const std::string& destMachineName,
                             const std::string& destPath,
                             const std::string& transferId):msrcUser(srcUser),
                             msrcMachineName(srcMachineName),
                             msrcPath(srcPath),
                             msrcUserKey(srcUserKey),
                             mdestUser(destUser),
                             mdestMachineName(destMachineName),
                             mdestPath(destPath),                            
                             mtransferId(transferId){}

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
return lastExecStatus;
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

 /* command << sshCommand << " -i " << privateKey << " -l " << userName;
  command << " -p " << sshPort << " " << server << " " << cmd;*/


 command << FileTransferServer::getSSHCommand()  << " -l " << getSrcUser();
  command << " -C"  << " -o BatchMode=yes " << " -o StrictHostKeyChecking=no";
  command << " -o ForwardAgent=yes";
 // command  << " -o ControlMaster=yes " << " -o ControlPath=/tmp/ssh-%r@%h:%p";
  command << " -p " << FileTransferServer::getSSHPort() << " " << getSrcMachineName() << " " << cmd;


  istringstream is(command.str());

  copy(istream_iterator<string>(is),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tokens));

/***********************************************/

vector<string>:: iterator ite;
cout << endl;
for (ite=tokens.begin(); ite!=tokens.end();++ite)
cout << *ite << endl;
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
  setProcessId (pid);

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
  lastExecStatus = status;
  cout << "result.second    :" << result.second << endl;
  return result;

}





