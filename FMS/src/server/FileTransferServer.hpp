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

class FileTransferServer{

  public:

    FileTransferServer(const SessionServer& sessionServer,const FMS_Data::FileTransfer& filetranfer, const int& vishnuId);


    FileTransferServer(const SessionServer& sessionServer,
                    const std::string& srcHost,
                   const std::string& destHost,
                   const std::string& srcFilePath,
                   const std::string& destFilePath,
                   const int& vishnuId);



    int addCpThread(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options);
    int addCpAsyncThread(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options);
    int  addMvThread();
    int  addMvAsyncThread();
  
    int stopThread(const std::string& thrId);
    const FMS_Data::FileTransfer& getFileTransfer() const; 
    void setFileTransfer( const FMS_Data::FileTransfer& fileTransfer) const; 
static Database * getDatabaseInstance();

  private:

    void wait ();
    void getUserInfo(std::string& name, std::string& userId); 
    int addTransferThread(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options);
    void copy(const SSHFile& file,const std::string& dest, const FMS_Data::CpFileOptions& options,const std::string& transferId);
    int insertIntoDatabase(int processId=-1);
    void updateData();
    void updateStatus(const FMS_Data::Status& status,const std::string& transferId);

    int mvishnuId;
    boost::thread mthread;
    SessionServer msessionServer;
    boost::mutex mmutex;
    mutable FMS_Data::FileTransfer mfileTransfer;
    SSHExec *ssh;

  // static DbFactory mfactory;
};

#endif
