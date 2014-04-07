
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
#include "fmsUtils.hpp"
#include "utilServer.hpp"

namespace ba = boost::algorithm;

// {{RELAX<MISRA_0_1_3> Two static variables
unsigned int FileTransferServer::msshPort = 22;
std::string FileTransferServer::msshCommand = "/usr/bin/ssh";

// }}RELAX<MISRA_0_1_3>

// Get The Database instance
Database*
FileTransferServer::getDatabaseInstance() {
  Database* databaseInstance (DbFactory().getDatabaseInstance());
  if (databaseInstance == NULL) {
    throw SystemException(ERRCODE_DBERR, "Get a null instance of database" );
  }
  return databaseInstance;
}

// Check The transfer id

void
FileTransferServer::checkTransferId(std::string transferId) {
  std::string sqlTransferRequest = "SELECT transferId from filetransfer where transferId='"+FileTransferServer::getDatabaseInstance()->escapeData(transferId)+"'";
  boost::scoped_ptr<DatabaseResult> transfer(FileTransferServer::getDatabaseInstance()->getResult(sqlTransferRequest.c_str()));
  if (transfer->getNbTuples()==0) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
  }
}

// add a query to the sql request

void
FileTransferServer::addOptionRequest(const std::string& name,
                                     const std::string& value,
                                     std::string& request) {
  request.append(" and "+name+"=");
  request.append("'"+FileTransferServer::getDatabaseInstance()->escapeData(value)+"'");
}


/**
 * \brief Function to check if a given user identifier exists
 * \param userId the user identifier
 * \return raises an exception on error
 */
void
FileTransferServer::checkUserId(std::string userId) {
  std::string sqlUserRequest = "SELECT userid from users where userid='"+FileTransferServer::getDatabaseInstance()->escapeData(userId)+"'";
  boost::scoped_ptr<DatabaseResult> user(FileTransferServer::getDatabaseInstance()->getResult(sqlUserRequest.c_str()));
  if (user->getNbTuples() == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_USERID);
  }
}

// A constructor with parameters
FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
                                       const int& vishnuId):
  mvishnuId(vishnuId),
  mtransferType(File::undefined),
  msessionServer(sessionServer) {

}

// Another constructor with parameters
FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
                                       const std::string& srcHost,
                                       const std::string& destHost,
                                       const std::string& srcFilePath,
                                       const std::string& destFilePath,
                                       const int& vishnuId):
  mvishnuId(vishnuId),
  mtransferType(File::undefined),
  msessionServer(sessionServer) {
  mfileTransfer.setSourceMachineId(srcHost);
  mfileTransfer.setDestinationMachineId(destHost);
  mfileTransfer.setSourceFilePath(srcFilePath);
  mfileTransfer.setDestinationFilePath(destFilePath);
}


// To get a user info from the database
void
FileTransferServer::getUserInfo(std::string& clientMachineName,
                                std::string& userId) {
  std::vector<std::string> result;
  std::vector<std::string>::const_iterator iter;
  std::string sessionId = msessionServer.getAttribut("where sessionkey='"+FileTransferServer::getDatabaseInstance()->escapeData((msessionServer.getData()).getSessionKey())+"'", "vsessionid");

  std::string sqlTransferRequest="SELECT name, userid,vsessionid from clmachine,users,vsession where vsession.clmachine_numclmachineid=clmachine.numclmachineid  "
                                 " and vsession.users_numuserid=users.numuserid and "
                                 "vsessionid='"+ FileTransferServer::getDatabaseInstance()->escapeData(sessionId)+"'";

  boost::scoped_ptr<DatabaseResult> transfer(FileTransferServer::getDatabaseInstance()->getResult(sqlTransferRequest.c_str()));

  if (transfer->getNbTuples() != 0) {
    for (size_t i = 0; i< transfer->getNbTuples(); ++i){
      result.clear();
      result=transfer->get(i);
      iter=result.begin();
      clientMachineName=*(iter);
      userId=*(++iter);

    }
  }
}

// To log data into database
void
FileTransferServer::updateDatabaseRecord()
{
  std::string sqlCond = boost::str(boost::format("WHERE sessionkey='%1%'")
                                   % getDatabaseInstance()->escapeData((msessionServer.getData()).getSessionKey())
                                   );

  Database* db = FileTransferServer::getDatabaseInstance();
  std::string sqlUpdate="UPDATE filetransfer set ";
  sqlUpdate+="vsession_numsessionid=" + msessionServer.getAttribut(sqlCond, "numsessionid") + ",";
  sqlUpdate+="userId='" + getDatabaseInstance()->escapeData(mfileTransfer.getUserId()) + "',";
  sqlUpdate+="clientMachineId='" + getDatabaseInstance()->escapeData(mfileTransfer.getClientMachineId()) + "',";
  sqlUpdate+="sourceMachineId='" + getDatabaseInstance()->escapeData(mfileTransfer.getSourceMachineId()) + "',";
  sqlUpdate+="destinationMachineId='" + db->escapeData(mfileTransfer.getDestinationMachineId()) + "',";
  sqlUpdate+="sourceFilePath='" + db->escapeData(mfileTransfer.getSourceFilePath()) + "',";
  sqlUpdate+="destinationFilePath='" + db->escapeData(mfileTransfer.getDestinationFilePath()) + "',";
  sqlUpdate+="status=" + vishnu::convertToString(mfileTransfer.getStatus()) + ",";
  sqlUpdate+="fileSize=" + vishnu::convertToString(mfileTransfer.getSize()) + ",";
  sqlUpdate+="trCommand=" + vishnu::convertToString(mfileTransfer.getTrCommand()) + ",";
  sqlUpdate+="processid=" + vishnu::convertToString(-1) + ",";
  sqlUpdate+="errorMsg='" + getDatabaseInstance()->escapeData(mfileTransfer.getErrorMsg()) + "',";
  sqlUpdate+="startTime=CURRENT_TIMESTAMP ";
  sqlUpdate+="WHERE transferid='" + FileTransferServer::getDatabaseInstance()->escapeData(mfileTransfer.getTransferId()) + "';";
  db->process(sqlUpdate);
}

// update file transfer data
void
FileTransferServer::updateData() {
  std::string clientMachineName;
  std::string userId;
  getUserInfo(clientMachineName, userId);

  std::string vishnuFileTransferId = vishnu::getObjectId(mvishnuId,
                                                         "formatidfiletransfer",
                                                         vishnu::FILETRANSFERT,
                                                         clientMachineName);

  mfileTransfer.setClientMachineId(clientMachineName);
  mfileTransfer.setUserId(userId);
  mfileTransfer.setTransferId(vishnuFileTransferId);
}

// To add a new file transfer thread
int
FileTransferServer::addTransferThread(const std::string& srcUser,
                                      const std::string& srcMachineName,
                                      const std::string& srcUserKey,
                                      const std::string& destUser,
                                      const std::string& destMachineName,
                                      const FMS_Data::CpFileOptions& options)
{
  updateData(); // update datas and get the vishnu transfer id
  int direction;
  if (vishnu::ifLocalTransferInvolved(srcMachineName, destMachineName, direction)) {
    mfileTransfer.setStatus(vishnu::TRANSFER_WAITING_CLIENT_RESPONSE);
    updateDatabaseRecord();
    LOG("[INFO] request transfer from client size", 1);
    return 0;
  }

  FMS_Data::CpFileOptions optionsCopy(options);
  int timeout(0);
  if (options.getTrCommand() == vishnu::UNDEFINED_TRANSFER_MANAGER) {
    std::string sessionId = msessionServer.getAttribut("where sessionkey='"+FileTransferServer::getDatabaseInstance()->escapeData((msessionServer.getData()).getSessionKey())+"'", "vsessionid");

    std::string query="SELECT users.numuserid,users_numuserid,vsessionid from users,vsession "
                      " WHERE vsession.users_numuserid=users.numuserid "
                      "  AND vsessionid='"+ FileTransferServer::getDatabaseInstance()->escapeData(sessionId)+"'";

    boost::scoped_ptr<DatabaseResult> dbResult(FileTransferServer::getDatabaseInstance()->getResult(query));

    if (dbResult->getNbTuples() != 0) {
      std::string numuserId= dbResult->getFirstElement();
      OptionValueServer optionValueServer;
      optionsCopy.setTrCommand(optionValueServer.getOptionValueForUser(numuserId, TRANSFERCMD_OPT));
      timeout = optionValueServer.getOptionValueForUser(numuserId, TRANSFER_TIMEOUT_OPT);
    }
  }

  boost::scoped_ptr<FileTransferCommand> transferManager(
        FileTransferCommand::getTransferManager(optionsCopy, timeout));

  boost::scoped_ptr<SSHFile> srcFileServer(
        new SSHFile(msessionServer,
                    mfileTransfer.getSourceFilePath(),
                    srcMachineName,
                    srcUser,
                    "",
                    srcUserKey,
                    "",
                    FileTransferServer::getSSHPort(),
                    FileTransferServer::getSSHCommand(),
                    transferManager->getLocation()));


  mfileTransfer.setTrCommand(optionsCopy.getTrCommand());
  mfileTransfer.setStatus(vishnu::TRANSFER_INPROGRESS); //INPPROGRESS

  if (! srcFileServer->exists() || ! srcFileServer->getErrorMsg().empty()) {
    mfileTransfer.setStatus(vishnu::TRANSFER_FAILED);
    mfileTransfer.setSize(0);
    mfileTransfer.setStartTime(0);
    mfileTransfer.setErrorMsg(srcFileServer->getErrorMsg());
    updateDatabaseRecord();
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,srcFileServer->getErrorMsg());
  }

  mfileTransfer.setSize(srcFileServer->getSize());
  mfileTransfer.setStartTime(0);

  if ( srcUser==destUser
       && srcMachineName == destMachineName
       && mfileTransfer.getSourceFilePath() == mfileTransfer.getDestinationFilePath())
  {
    mfileTransfer.setStatus(vishnu::TRANSFER_FAILED); //failed
    mfileTransfer.setErrorMsg("same source and destination");
    updateDatabaseRecord();
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "same source and destination ");
  }

  // create a TransferExec instance
  TransferExec transferExec(msessionServer,
                            srcUser,
                            srcMachineName,
                            mfileTransfer.getSourceFilePath(),
                            srcUserKey,
                            destUser,
                            destMachineName,
                            mfileTransfer.getDestinationFilePath(),
                            mfileTransfer.getTransferId());

  updateDatabaseRecord();

  // create the thread to perform the copy
  if (mtransferType == File::copy) {
    mthread = boost::thread(&FileTransferServer::copy,
                            this,
                            transferExec,
                            transferManager->getCommand());
  } else if (mtransferType == File::move) {
    mthread = boost::thread(&FileTransferServer::move,
                            this,
                            transferExec,
                            transferManager->getCommand());
  }

  return 0;
}



// To perform a file copy
void
FileTransferServer::copy(const TransferExec& transferExec,
                         const std::string& trCmd) {
  //build the destination complete path
  std::ostringstream destCompletePath;

  destCompletePath << transferExec.getDestUser() << "@"<< transferExec.getDestMachineName() <<":"<< transferExec.getDestPath();
  std::pair<std::string,std::string> trResult;

  trResult = transferExec.exec(trCmd + " " +transferExec.getSrcPath()+" "+destCompletePath.str() );

  if (trResult.second.find("Warning") != std::string::npos
      || trResult.first.find("Warning")!=std::string::npos) {
    trResult = transferExec.exec(trCmd + " " +transferExec.getSrcPath()+" "+destCompletePath.str());
  }

  // Clean the output message
  std::string allOutputMsg (FileTransferServer::cleanOutputMsg(trResult.first+trResult.second));

  if (allOutputMsg.length() != 0) {
    updateStatus(vishnu::TRANSFER_FAILED, transferExec.getTransferId(), allOutputMsg);

  } else {
    updateStatus(vishnu::TRANSFER_COMPLETED,transferExec.getTransferId(),"");
  }
}


void
FileTransferServer::move(const TransferExec& transferExec,
                         const std::string& trCmd) {
  // perform the copy
  copy(transferExec,trCmd);
  int lastExecStatus=transferExec.getLastExecStatus();

  if (lastExecStatus == 0) {

    // remove the source file
    FileFactory ff;
    ff.setSSHServer(transferExec.getSrcMachineName());
    boost::scoped_ptr<File> file (ff.getFileServer( transferExec.getSessionServer(),transferExec.getSrcPath(),
                                                    transferExec.getSrcUser(),transferExec.getSrcUserKey() ) ) ;
    try {
      FMS_Data::RmFileOptions options;
      options.setIsRecursive("true");
      file->rm(options);
    } catch(VishnuException& err) {
      updateStatus(vishnu::TRANSFER_FAILED,transferExec.getTransferId(),err.what());
      transferExec.setLastExecStatus(1);
    }
  }
}

// To update file transfer status into database
void
FileTransferServer::updateStatus(const FMS_Data::Status& status,
                                 const std::string& transferId,
                                 const std::string& errorMsg)
{
  std::string sqlUpdateRequest = "UPDATE filetransfer"
                                 " SET status="+vishnu::convertToString(status)+
                                 ", errorMsg='"+getDatabaseInstance()->escapeData(errorMsg)+"'"+
                                 " WHERE transferid='"+getDatabaseInstance()->escapeData(transferId)+"'"
                                 + " and status<>2";
  FileTransferServer::getDatabaseInstance()->process(sqlUpdateRequest);
}

// get error message from database
std::string
FileTransferServer::getErrorFromDatabase(const std::string& transferid) {
  std::string sqlCommand = "SELECT errormsg from filetransfer where transferid='"+ FileTransferServer::getDatabaseInstance()->escapeData(transferid) +"'";

  boost::scoped_ptr<DatabaseResult> result(FileTransferServer::getDatabaseInstance()->getResult(sqlCommand.c_str()));

  return result->getFirstElement();
}

/**
 * \brief Clean an output message
 * remove the line beginning by "Pseudo-terminal"
 */
std::string
FileTransferServer::cleanOutputMsg(const std::string& allOutputMsg) {
  if (ba::starts_with(allOutputMsg,"Pseudo-terminal")) {
    size_t pos =allOutputMsg.find_first_of("\n");

    if (pos != std::string::npos) {
      return allOutputMsg.substr(pos+1);
    }
  }
  return allOutputMsg;
}


// A file copy thread
int
FileTransferServer::addCpThread(const std::string& srcUser,
                                const std::string& srcMachineName,
                                const std::string& srcUserKey,
                                const std::string& destUser,
                                const std::string& destMachineName,
                                const FMS_Data::CpFileOptions& options) {
  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
  waitThread();

  std::string errorMsg(getErrorFromDatabase(mfileTransfer.getTransferId()));

  if (false == errorMsg.empty()) {
    throw FMSVishnuException (ERRCODE_RUNTIME_ERROR,errorMsg);
  }
  return 0;
}


// add a new synchronous file copy thread
int
FileTransferServer::addCpAsyncThread(const std::string& srcUser,
                                     const std::string& srcMachineName,
                                     const std::string& srcUserKey,
                                     const std::string& destUser,
                                     const std::string& destMachineName,
                                     const FMS_Data::CpFileOptions& options) {
  mtransferType=File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
  return 0;
}


// add a new synchronous move file  thread
int
FileTransferServer::addMvThread(const std::string& srcUser,
                                const std::string& srcMachineName,
                                const std::string& srcUserKey,
                                const std::string& destUser,
                                const std::string& destMachineName,
                                const FMS_Data::CpFileOptions& options) {
  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);
  waitThread();

  std:: string errorMsg(getErrorFromDatabase(mfileTransfer.getTransferId()));

  if (false == errorMsg.empty()) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,errorMsg);
  }
  return 0;
}

// add a new asynchronous file move thread


int
FileTransferServer::addMvAsyncThread(const std::string& srcUser,
                                     const std::string& srcMachineName,
                                     const std::string& srcUserKey,
                                     const std::string& destUser,
                                     const std::string& destMachineName,
                                     const FMS_Data::CpFileOptions& options) {
  mtransferType=File::move;
  FMS_Data::CpFileOptions mvOptions(options);
  mvOptions.setIsRecursive(true);
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName,mvOptions);
  return 0;
}

// Wait until a thread terminates
void
FileTransferServer::waitThread() {
  mthread.join();
}


/**
 * \brief Function to treat the Stop file transfer options
 * \param options the object which contains the Stop file transfers options values
 * \param sqlRequest the sql data base request
 * \return raises an exception on error
 */
void
FileTransferServer::processOptions(const FMS_Data::StopTransferOptions& options,
                                   std::string& sqlRequest) {

  std::string transferId(options.getTransferId()),machineName(options.getFromMachineId()),userId(options.getUserId());

  //To check if the transferId is defined
  if (transferId.size() != 0) {

    if (transferId.compare("all")!=0 && transferId.compare("ALL")!=0){
      //To check the transfer Id
      FileTransferServer::checkTransferId(options.getTransferId());
      //To add the transferId on the request
      FileTransferServer::addOptionRequest("transferId", options.getTransferId(), sqlRequest);

    }

  }

  //To check if the fromMachineId is defined
  if (machineName.size() != 0) {

    sqlRequest.append(" and (sourceMachineId='"+FileTransferServer::getDatabaseInstance()->escapeData(machineName)+"'"+" or destinationMachineId='"+FileTransferServer::getDatabaseInstance()->escapeData(machineName)+"')");
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

    if (userId.compare("all")!=0 && userId.compare("ALL")!=0){
      //To check the user Id
      FileTransferServer::checkUserId(options.getUserId());

      //To add the userId on the request
      FileTransferServer::addOptionRequest("userId", userId, sqlRequest);
    }
  } else {
    FileTransferServer::addOptionRequest("userId", userServer.getData().getUserId(), sqlRequest);
  }
}

// Cancel  file transfers
int
FileTransferServer::stopThread(const FMS_Data::StopTransferOptions& options) {


  if (! options.getTransferId().empty()
      || ! options.getUserId().empty()
      || ! options.getFromMachineId().empty()) {

    std::string sqlListOfPid = "SELECT transferId,processId "
                               " FROM filetransfer, vsession "
                               " WHERE vsession.numsessionid=filetransfer.vsession_numsessionid "
                               " AND filetransfer.status=0";

    std::string transferid;
    std::vector<std::string>::iterator iter;
    std::vector<std::string> results;

    msessionServer.check();

    processOptions(options, sqlListOfPid);

    boost::scoped_ptr<DatabaseResult> ListOfPid (FileTransferServer::getDatabaseInstance()->getResult(sqlListOfPid.c_str()));

    if (ListOfPid->getNbTuples() != 0){
      for (size_t i = 0; i < ListOfPid->getNbTuples(); ++i) {
        int pid;
        results.clear();
        results = ListOfPid->get(i);
        iter = results.begin();
        transferid = *iter;
        ++iter;
        pid = vishnu::convertToInt(*iter);
        stopThread(transferid,pid);
        ++iter;
      }

    } else {
      throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "There is no file transfer in progress ");
    }
  }

  return 0;
}



// cancel a file transfer


int
FileTransferServer::stopThread(const std::string& transferid,const int& pid) {
  int result=0;

  if (pid != -1) {
    result = kill(pid, SIGKILL);

    if (result) {
      updateStatus(vishnu::TRANSFER_FAILED, transferid, strerror(errno));
      throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,strerror(errno));
    }

    // get the user responsible for the stop request

    std::string sessionId = msessionServer.getAttribut("where sessionkey='"+FileTransferServer::getDatabaseInstance()->escapeData((msessionServer.getData()).getSessionKey())+"'", "vsessionid");
    std::string sqlCommand="SELECT userid,vsessionid "
                           " FROM users,vsession "
                           " WHERE vsession.users_numuserid=users.numuserid"
                           " AND vsessionid='"+ FileTransferServer::getDatabaseInstance()->escapeData(sessionId)+"'";

    boost::scoped_ptr<DatabaseResult> dbResult(FileTransferServer::getDatabaseInstance()->getResult(sqlCommand));
    std::string logMsg= "by: "+ dbResult->getFirstElement();

    updateStatus(vishnu::TRANSFER_CANCELLED, transferid, logMsg);
  }
  return result;
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
                            const std::string& transferId):
  mtransferId(transferId),
  msessionServer(sessionServer),
  msrcUser(srcUser),
  msrcMachineName(srcMachineName),
  msrcPath(srcPath),
  msrcUserKey(srcUserKey),
  mdestUser(destUser),
  mdestMachineName(destMachineName),
  mdestPath(destPath) {

  mprocessId=-1;
  mlastExecStatus=0;
}

// Set the file transfer process identifier into database
void
TransferExec::updatePid(const int& pid) const {
  setProcessId(pid);
  std::string query = (boost::format("UPDATE filetransfer SET processid=%1%"
                                     " WHERE transferId='%2%';")
                       %vishnu::convertToString(pid)
                       %getTransferId()).str();
  FileTransferServer::getDatabaseInstance()->process(query);
}

// Perform a remote command through ssh
std::pair<std::string, std::string>
TransferExec::exec(const std::string& cmd) const {
  std::vector<std::string> tokens;
  std::ostringstream trCommand;
  pid_t pid;
  std::pair<std::string, std::string> result;
  int transferComPipeOut[2];
  int transferComPipeErr[2];
  int status;

  // build the transfer command
  trCommand  << FileTransferServer::getSSHCommand() << " -t -q " << " -l " << getSrcUser();
  trCommand <<" -C "  << " -o BatchMode=yes " << " -o StrictHostKeyChecking=no";
  trCommand << " -o ForwardAgent=yes";
  trCommand << " -p " << FileTransferServer::getSSHPort() << " " << getSrcMachineName() << " " << cmd;

  // Exec The transfer command

  std::istringstream is(trCommand.str());

  copy(std::istream_iterator<std::string>(is),
       std::istream_iterator<std::string>(),
       std::back_inserter<std::vector<std::string> >(tokens));



  char* argv[tokens.size()+1];
  argv[tokens.size()]=NULL;

  for (unsigned int i=0; i<tokens.size(); ++i){
    argv[i]=strdup(tokens[i].c_str());
  }

  if (pipe(transferComPipeOut)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i){
      free(argv[i]);
    }
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "Error creating communication pipe");
  }

  if (pipe(transferComPipeErr)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i){
      free(argv[i]);
    }
    close(transferComPipeOut[0]);
    close(transferComPipeOut[1]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating communication pipe");
  }

  pid = fork();
  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i){
      free(argv[i]);
    }
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error forking process");
  }
  if (pid == 0) {
    close(transferComPipeOut[0]); /* Close unused read end */
    close(transferComPipeErr[0]); /* Close unused write end */
    dup2(transferComPipeOut[1], 1);
    dup2(transferComPipeErr[1], 2);
    close(transferComPipeOut[1]);
    close(transferComPipeErr[1]);
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }

  // Store the child process id
  updatePid (pid);

  close(transferComPipeOut[1]); /* Close unused write end */

  close(transferComPipeErr[1]);/* Close unused write end */


  char c;
  while (read(transferComPipeOut[0], &c, 1)){
    result.first+=c;
  }

  while (read(transferComPipeErr[0], &c, 1)){
    result.second+=c;
  }

  if (waitpid(pid, &status, 0)==-1) {
    close(transferComPipeOut[0]);
    close(transferComPipeErr[0]);
    for (unsigned int i=0; i<tokens.size(); ++i){
      free(argv[i]);
    }
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error executing command "+trCommand.str());
  }

  close(transferComPipeOut[0]);
  close(transferComPipeErr[0]);
  for (unsigned int i=0; i<tokens.size(); ++i){
    free(argv[i]);
  }
  mlastExecStatus = status;

  return result;
}
