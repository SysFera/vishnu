#include "FileTransferProxy.hpp"


FileTransferProxy::FileTransferProxy(const FMS_Data::FileTransfer& filetranfer):mfileTransfer(fileTransfer){}

long FileTransferProxy::addCpThread();


long FileTransferProxy::addMvThread();



int FileTransferProxy::stopThread(long thrId);


