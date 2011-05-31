#ifndef FILETRANSFERSERVER
#define FILETRANSFERSERVER
/**
 * \file FileTransferServer.hpp
 * \brief Contains a declaration of all File transfer services
 */

#include "FMS_Data.hpp"
#include <string>
#include <boost/thread.hpp>
#include "DbFactory.hpp"
#include "SessionServer.hpp"
class FileTransferServer{

  public:

    FileTransferServer(const SessionServer& sessionServer,const FMS_Data::FileTransfer& filetranfer);


    FileTransferServer(const SessionServer& sessionServer,
                    const std::string& srcHost,
                   const std::string& destHost,
                   const std::string& srcFilePath,
                   const std::string& destFilePath);


    int addCpThread();
    int  addMvThread();
    int addCpAsyncThread();
    int  addMvAsyncThread();
  
    int stopThread(const std::string& thrId);
    const FMS_Data::FileTransfer& getFileTransfer() const; 
    void setFileTransfer( const FMS_Data::FileTransfer& fileTransfer) const; 


  private:

 boost::thread mthread;
 SessionServer msessionServer;
 void wait ();
void getUserInfo(std::string& name, std::string& userId); 
 int insertIntoDatabase();
mutable FMS_Data::FileTransfer mfileTransfer;
Database *mdatabaseVishnu;
};

#endif
