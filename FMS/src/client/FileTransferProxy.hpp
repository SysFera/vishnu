#ifndef FILETRANSFERPROXY
#define FILETRANSFERPROXY
/**
 * \file FileTransferProxy.hpp
 * \brief Contains a declaration of all File transfer services
 */

#include "FMS_Data.hpp"

class FileTransferProxy{

  public:
  
    FileTransferProxy(const FMS_Data::FileTransfer& filetranfer);
    long addCpThread();
    long addMvThread();
    int stopThread(long thrId);
  
  private:

    FMS_Data::FileTransfer mfileTransfer;
};

#endif
