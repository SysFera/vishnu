#ifndef FILETRANSFERPROXY
#define FILETRANSFERPROXY
/**
 * \file FileTransferProxy.hpp
 * \brief Contains a declaration of all File transfer services
 */

#include "FMS_Data_forward.hpp"
#include "FMS_Data.hpp"

class FileTransferProxy{

  public:
 
    FileTransferProxy(const std::string& sessionKey);
 
    FileTransferProxy(const std::string& sessionKey,
                      const std::string& srcFilePath,
                      const std::string& destFilePath);

    int addCpThread(const FMS_Data::CpFileOptions& options);

    int addMvThread(const FMS_Data::CpFileOptions& options);

    int addCpAsyncThread(const FMS_Data::CpFileOptions& options);

    int addMvAsyncThread(const FMS_Data::CpFileOptions& options);


    int stopThread(const FMS_Data::StopTransferOptions& options);

    const FMS_Data::FileTransfer& getFileTransfer() const;

    ~FileTransferProxy(); 

  private:

    std::string msessionKey;
    std::string msrcFilePath;
    std::string mdestFilePath;
    FMS_Data::FileTransfer mtransferInfo;
};

#endif
