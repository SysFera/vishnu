#include "FileTransferProxy.hpp"


FileTransferProxy::FileTransferProxy(const FMS_Data::FileTransfer& filetranfer);

long FileTransferProxy::addCpThread();


long FileTransferProxy::addMvThread();



int FileTransferProxy::stopThread(long thrId);


