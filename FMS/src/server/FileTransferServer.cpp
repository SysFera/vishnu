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
#include "fmsUtils.hpp"
#include "utilServer.hpp"
#include "Logger.hpp"

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
  std::string query = (boost::format("SELECT numfiletransferid"
                                     " FROM filetransfer"
                                     " WHERE numfiletransferid=%1%")
                       % vishnu::convertToLong(transferId)).str();

  boost::scoped_ptr<DatabaseResult> transfer(mdatabase->getResult(query));

  if (transfer->getNbTuples()==0) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
  }
}

// add a query to the sql request

void
FileTransferServer::addOptionRequest(const std::string& name,
                                     const std::string& value,
                                     std::string& request) {
  request.append(" AND "+name+"=");
  request.append("'"+mdatabase->escapeData(value)+"'");
}

// A constructor with parameters
FileTransferServer::FileTransferServer(const SessionServer& sessionServer)
  : mdatabase( getDatabaseInstance() ),
    mtransferType(File::undefined),
    msessionServer(sessionServer)
{

}

// Another constructor with parameters
FileTransferServer::FileTransferServer(const SessionServer& sessionServer,
                                       const std::string& srcHost,
                                       const std::string& destHost,
                                       const std::string& srcFilePath,
                                       const std::string& destFilePath)
  : mdatabase( getDatabaseInstance() ),
    mtransferType(File::undefined),
    msessionServer(sessionServer)
{
  mfileTransfer.setSourceMachineId(srcHost);
  mfileTransfer.setDestinationMachineId(destHost);
  mfileTransfer.setSourceFilePath(srcFilePath);
  mfileTransfer.setDestinationFilePath(destFilePath);
}

/**
 * @brief return a pair <machineid, userid> corresponding to the user session
 * @return
 */
std::pair<std::string, std::string>
FileTransferServer::getMachineUserPair(void)
{

  std::vector<std::string> result;
  std::vector<std::string>::const_iterator iter;
  std::string sessionId = msessionServer.getAttributFromSessionKey(msessionServer.getData().getSessionKey(), "vsessionid");
  std::string query=boost::str(
                      boost::format("SELECT name, userid, vsessionid"
                                    " FROM clmachine, users, vsession"
                                    " WHERE vsession.clmachine_numclmachineid=clmachine.numclmachineid  "
                                    "  AND vsession.users_numuserid=users.numuserid "
                                    "  AND vsessionid='%1%';")
                      % mdatabase->escapeData(sessionId));

  boost::scoped_ptr<DatabaseResult> transfer(mdatabase->getResult(query));

  std::string clientMachineName = "";
  std::string userId = "";
  if (transfer->getNbTuples() != 0) {
    for (size_t i = 0; i< transfer->getNbTuples(); ++i){
      result.clear();
      result=transfer->get(i);
      iter=result.begin();
      clientMachineName=*(iter);
      userId=*(++iter);
    }
  }

  return std::pair<std::string, std::string>(clientMachineName, userId);
}

// To log data into database
void
FileTransferServer::updateDatabaseRecord()
{
  std::string numSession = msessionServer.getAttributFromSessionKey( msessionServer.getData().getSessionKey(), "numsessionid" );
  std::string sqlUpdate="UPDATE filetransfer SET ";
  sqlUpdate+="vsession_numsessionid=" + numSession + ",";
  sqlUpdate+="clientMachineId='" + mdatabase->escapeData(mfileTransfer.getClientMachineId()) + "',";
  sqlUpdate+="sourceMachineId='" + mdatabase->escapeData(mfileTransfer.getSourceMachineId()) + "',";
  sqlUpdate+="destinationMachineId='" + mdatabase->escapeData(mfileTransfer.getDestinationMachineId()) + "',";
  sqlUpdate+="sourceFilePath='" + mdatabase->escapeData(mfileTransfer.getSourceFilePath()) + "',";
  sqlUpdate+="destinationFilePath='" + mdatabase->escapeData(mfileTransfer.getDestinationFilePath()) + "',";
  sqlUpdate+="status=" + vishnu::convertToString(mfileTransfer.getStatus()) + ",";
  sqlUpdate+="fileSize=" + vishnu::convertToString(mfileTransfer.getSize()) + ",";
  sqlUpdate+="trCommand=" + vishnu::convertToString(mfileTransfer.getTrCommand()) + ",";
  sqlUpdate+="processid=" + vishnu::convertToString(-1) + ",";
  sqlUpdate+="errorMsg='" + mdatabase->escapeData(mfileTransfer.getErrorMsg()) + "',";
  sqlUpdate+="startTime=CURRENT_TIMESTAMP ";
  sqlUpdate+="WHERE numfiletransferid='" + mdatabase->escapeData(mfileTransfer.getTransferId()) + "';";
  mdatabase->process(sqlUpdate);
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
  std::pair<std::string, std::string> machineUserPair = getMachineUserPair();
  mfileTransfer.setClientMachineId(machineUserPair.first);
  mfileTransfer.setUserId(machineUserPair.second);

  int direction;
  if (vishnu::ifLocalTransferInvolved(srcMachineName, destMachineName, direction)) {

    mfileTransfer.setStatus(vishnu::TRANSFER_WAITING_CLIENT_RESPONSE);

    dbSave();

    if (destUser.empty()) {
      std::string tmp = mfileTransfer.getSourceFilePath();
      mfileTransfer.setSourceFilePath(srcUser+"@"+srcMachineName+":"+tmp);
    } else {
      std::string tmp = mfileTransfer.getDestinationFilePath();
      mfileTransfer.setDestinationFilePath(destUser+"@"+destMachineName+":"+tmp);
    }

    LOG("[INFO] request transfer from client side", LogInfo);
    return 0;
  }

  FMS_Data::CpFileOptions optionsCopy(options);
  int timeout (0); //FIXME: get timeout from config

  boost::scoped_ptr<FileTransferCommand> transferManager( FileTransferCommand::getTransferManager(optionsCopy, timeout) );

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

    dbSave();

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

    dbSave();

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

  dbSave();

  // create the thread to perform the copy
  if (mtransferType == File::copy) {
    mthread = boost::thread(&FileTransferServer::copy, this, transferExec, transferManager->getCommand());
  } else if (mtransferType == File::move) {
    mthread = boost::thread(&FileTransferServer::move, this, transferExec, transferManager->getCommand());
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
  std::string query = "UPDATE filetransfer"
                      " SET status="+vishnu::convertToString(status)+
                      ", errorMsg='"+mdatabase->escapeData(errorMsg)+"'"+
                      " WHERE numfiletransferid='"+mdatabase->escapeData(transferId)+"'"
                      + " and status<>2";
  mdatabase->process(query);
}

// get error message from database
std::string
FileTransferServer::getErrorFromDatabase(const std::string& transferid)
{
  std::string query = "SELECT errormsg "
                      " FROM filetransfer "
                      " WHERE numfiletransferid='"+ mdatabase->escapeData(transferid) +"'";

  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(query.c_str()));

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
  mtransferType = File::copy;
  addTransferThread(srcUser,srcMachineName,srcUserKey, destUser, destMachineName, options);
  waitThread();

  std::string errorMsg = getErrorFromDatabase(mfileTransfer.getTransferId());

  if (false == errorMsg.empty()) {
    throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, errorMsg);
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
  mtransferType = File::copy;
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
  mtransferType = File::move;
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
                                   std::string& query) {

  std::string transferIdOption = options.getTransferId();
  std::string machineAddrOption = options.getFromMachineId();
  std::string userIdOption = options.getUserId();

  //To check if the transferId is defined
  if (! transferIdOption.empty()) {
    if (transferIdOption != "all" && transferIdOption != "ALL") {
      checkTransferId( options.getTransferId() );
      addOptionRequest("transferId", options.getTransferId(), query);
    }
  }

  //To check if the fromMachineId is defined
  if (! machineAddrOption.empty()) {
    query.append(" and (sourceMachineId='"+mdatabase->escapeData(machineAddrOption)+"'"+" or destinationMachineId='"+FileTransferServer::getDatabaseInstance()->escapeData(machineAddrOption)+"')");
  }

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);

  userServer.init();

  //To check if the userId is defined
  if (userIdOption.empty()) {
    addOptionRequest("users.userId", userServer.getData().getUserId(), query);
  } else {
    if (! userServer.isAdmin()) {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }

    if (userIdOption != "all" && userIdOption != "ALL") {
      userServer.checkUserId(userIdOption);
      addOptionRequest("users.userId", userIdOption, query);
    }
  }
}

// Cancel  file transfers
int
FileTransferServer::stopThread(const FMS_Data::StopTransferOptions& options)
{
  if (! options.getTransferId().empty()
      || ! options.getUserId().empty()
      || ! options.getFromMachineId().empty()) {

    std::string query4Pid = (boost::format(
                               "SELECT numfiletransferid, processId "
                               " FROM filetransfer, vsession, users"
                               " WHERE filetransfer.status      = %1%"
                               "   AND vsession.numsessionid    = filetransfer.vsession_numsessionid"
                               "   AND vsession.users_numuserid = users.numuserid"
                               )
                             % vishnu::STATUS_ACTIVE).str();

    std::string transferid;
    std::vector<std::string>::iterator iter;
    std::vector<std::string> results;

    msessionServer.check();

    processOptions(options, query4Pid);

    boost::scoped_ptr<DatabaseResult> ListOfPid (mdatabase->getResult(query4Pid.c_str()));

    if (ListOfPid->getNbTuples() == 0){
      throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "There is no file transfer in progress ");
    }

    for (size_t i = 0; i < ListOfPid->getNbTuples(); ++i) {
      int pid;
      results.clear();
      results = ListOfPid->get(i);
      iter = results.begin();
      transferid = *iter;
      ++iter;
      pid = vishnu::convertToInt(*iter);
      stopThread(transferid, pid);
      ++iter;
    }
  }

  return 0;
}



// cancel a file transfer


int
FileTransferServer::stopThread(const std::string& transferid,const int& pid)
{
  int result = 0;

  if (pid != -1) {
    result = kill(pid, SIGKILL);

    if (result) {
      updateStatus(vishnu::TRANSFER_FAILED, transferid, strerror(errno));
      throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,strerror(errno));
    }

    // get the user responsible for the stop request

    std::string query = boost::str(
                          boost::format(
                            "SELECT users.userid, vsessionid "
                            " FROM users, vsession "
                            " WHERE vsession.sessionkey      = '%1%'"
                            "   AND vsession.users_numuserid = users.numuserid;"
                            )
                          % mdatabase->escapeData( msessionServer.getData().getSessionKey()) );

    boost::scoped_ptr<DatabaseResult> dbResult(mdatabase->getResult(query));

    std::string logMsg= "BY: "+ dbResult->getFirstElement();

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
                            const std::string& transferId)
  : mdatabase(FileTransferServer::getDatabaseInstance()),
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
                                     " WHERE numfiletransferid=%2%;")
                       % vishnu::convertToString(pid)
                       % getTransferId()).str();

  mdatabase->process(query);
}

// Perform a remote command through ssh
std::pair<std::string, std::string>
TransferExec::exec(const std::string& cmd) const
{
  std::string command = boost::str(boost::format("%1% -l %2% -C -o BatchMode=yes "
                                                 " -o StrictHostKeyChecking=no"
                                                 " -o ForwardAgent=yes"
                                                 " -p %3% %4% %5%"
                                                 )
                                   % FileTransferServer::getSSHCommand()
                                   % getSrcUser()
                                   % FileTransferServer::getSSHPort()
                                   % getSrcMachineName()
                                   % cmd);

  std::string output;
  std::pair<std::string, std::string> result;
  if (! vishnu::execSystemCommand(command, output)) { // error
    result.second = output;
  } else { // success
    result.first = output;
  }
  return result;
}



/**
 * @brief Insert the current transfer info into database
 * @return void. The transfer id will be recored in the encapsulated transfer object description
 */
void
FileTransferServer::dbSave(void)
{
  std::string numSession = msessionServer.getAttributFromSessionKey(msessionServer.getData().getSessionKey(), "numsessionid");
  std::string query = boost::str(
                        boost::format("INSERT INTO filetransfer"
                                      " (vsession_numsessionid, clientMachineId, sourceMachineId,"
                                      " destinationMachineId, sourceFilePath, destinationFilePath,"
                                      " status, fileSize, trCommand, processid, errorMsg, startTime)"
                                      " VALUES(%1%,'%2%','%3%','%4%','%5%','%6%',%7%,%8%,%9%,%10%,'%11%',%12%)")
                        % numSession
                        % mdatabase->escapeData(mfileTransfer.getClientMachineId())
                        % mdatabase->escapeData(mfileTransfer.getSourceMachineId())
                        % mdatabase->escapeData(mfileTransfer.getDestinationMachineId())
                        % mdatabase->escapeData(mfileTransfer.getSourceFilePath())
                        % mdatabase->escapeData(mfileTransfer.getDestinationFilePath())
                        % vishnu::convertToString(mfileTransfer.getStatus())
                        % vishnu::convertToString(mfileTransfer.getSize())
                        % vishnu::convertToString(mfileTransfer.getTrCommand())
                        % vishnu::convertToString(-1)
                        % mdatabase->escapeData(mfileTransfer.getErrorMsg())
                        % "CURRENT_TIMESTAMP");


  std::pair<int, u_int64_t> result = mdatabase->process(query);

  mfileTransfer.setTransferId( vishnu::convertToString(result.second) );
}
