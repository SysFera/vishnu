#include <boost/scoped_ptr.hpp>

#include "FileTransferProxy.hpp"
#include "FileProxyFactory.hpp"
#include "SessionProxy.hpp"

using namespace FMS_Data;
using namespace UMS_Data;
using namespace std;

FileTransferProxy::FileTransferProxy(const std::string& sessionKey):msessionKey(sessionKey){
}

FileTransferProxy::FileTransferProxy(const std::string& sessionKey,
                                     const std::string& srcFilePath,
                                     const std::string& destFilePath): msessionKey(sessionKey),
 msrcFilePath(srcFilePath), mdestFilePath(destFilePath){
}

int FileTransferProxy::addCpThread(const CpFileOptions& options){
  
  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->cpAsync(mdestFilePath,options,mtransferInfo);

  return result;

}

int FileTransferProxy::addMvThread(const CpFileOptions& options){

  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->mvAsync(mdestFilePath,options,mtransferInfo);

  return result;
}

int FileTransferProxy::stopThread(const StopTransferOptions& options) {

}

const FMS_Data::FileTransfer& FileTransferProxy::getFileTransfer() const {
  return mtransferInfo;
}

FileTransferProxy::~FileTransferProxy(){

}
