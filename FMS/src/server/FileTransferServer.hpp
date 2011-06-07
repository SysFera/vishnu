#ifndef FILETRANSFERSERVER_HPP
#define FILETRANSFERSERVER_HPP
/**
 * \file FileTransferServer.hpp
 * \brief Contains a declaration of all File transfer services
 */

#include "FMS_Data.hpp"
#include <string>
#include <boost/thread.hpp>
#include "DbFactory.hpp"
#include "SessionServer.hpp"
#include "SSHFile.hh"

/**
 * \brief A useful class to perform a transfer command
 */


class TransferExec{


  public:

    TransferExec (const SessionServer& sessionServer,
        const std::string& srcUser,
        const std::string& srcMachineName,
        const std::string& srcPath,
        const std::string& srcUserKey,
        const std::string& destUser,
        const std::string& destMachineName,
        const  std::string& destPath,
        const std::string& transferId);

    const int& getLastExecStatus() const;

    const SessionServer& getSessionServer() const;
    const std::string& getSrcUser() const;
    const std::string& getSrcMachineName() const;
    const std::string& getSrcPath() const;
    const std::string& getSrcUserKey() const;
    const std::string& getDestUser() const;
    const std::string& getDestMachineName() const;
    const std::string& getDestPath() const;
    const std::string& getTransferId() const;

    const int& getProcessId() const;
    void setProcessId(const int& processId)const;
    void updatePid(const int& pid)const;
    void setLastExecStatus(const int& status) const;
    std::pair<std::string, std::string> exec(const std::string& cmd) const;


  private:
    mutable int mlastExecStatus;
    mutable int mprocessId;
    mutable std::string mtransferId;

    SessionServer msessionServer;
    std::string msrcUser;
    std::string msrcMachineName;
    std::string msrcPath;
    std::string msrcUserKey;
    std::string mdestUser;
    std::string mdestMachineName;
    std::string mdestPath;


};



class FileTransferServer{

  public:

    FileTransferServer(const SessionServer& sessionServer, const int& vishnuId);


    FileTransferServer(const SessionServer& sessionServer,
        const std::string& srcHost,
        const std::string& destHost,
        const std::string& srcFilePath,
        const std::string& destFilePath,
        const int& vishnuId);


    int addCpThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options);


    int addCpAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options);

    int  addMvThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options);

    int  addMvAsyncThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options);

    int stopThread(const StopTransferOptions& options);
    const FMS_Data::FileTransfer& getFileTransfer() const; 
    void setFileTransfer( const FMS_Data::FileTransfer& fileTransfer) const; 

    static Database * getDatabaseInstance();
    static void setSSHPort(const unsigned int sshPort);
    static void setSSHCommand(const std::string& sshCommand);

    static const unsigned int getSSHPort();
    static const std::string& getSSHCommand( );
    
    static std::string filterString(  const std::string& toFilter);

  private:

    void waitThread ();
    void getUserInfo(std::string& name, std::string& userId); 

    int addTransferThread(const std::string& srcUser,const std::string& srcMachineName, const std::string& srcUserKey, const std::string& destUser, const std::string& destMachineName,const FMS_Data::CpFileOptions& options);
    void copy(const TransferExec& transferExec, const std::string& trCmd);
    void move(const TransferExec& transferExec, const std::string& trCmd);
    int stopThread(const int& pid);
    int logIntoDatabase(int processId=-1,const std::string& errorMsg="");
    void updateData();
    void updateStatus(const FMS_Data::Status& status,const std::string& transferId,const std::string& errorMsg);

    int mvishnuId;
    File::TransferType mtransferType;
    boost::thread mthread;
    SessionServer msessionServer;
    boost::mutex mmutex;
    mutable FMS_Data::FileTransfer mfileTransfer;

    static unsigned int msshPort;
    static std::string msshCommand;

    //TransferExec transferExec;

    // static DbFactory mfactory;
};




#endif
