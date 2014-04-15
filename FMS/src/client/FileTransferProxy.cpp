#include <boost/scoped_ptr.hpp>

#include "FileTransferProxy.hpp"
#include "FileProxyFactory.hpp"
#include "SessionProxy.hpp"
#include "utilClient.hpp"
#include "FMSServices.hpp"
#include "DIET_client.h"

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

  int result= f->cp(mdestFilePath,options);

  return result;

}

int FileTransferProxy::addMvThread(const CpFileOptions& options){

  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->mv(mdestFilePath,options);

  return result;
}

int FileTransferProxy::addCpAsyncThread(const CpFileOptions& options){

  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> fileProxy (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= fileProxy->cpAsync(mdestFilePath,options,mtransferInfo);

  return result;

}

int FileTransferProxy::addMvAsyncThread(const CpFileOptions& options){

  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->mvAsync(mdestFilePath,options,mtransferInfo);

  return result;
}



int FileTransferProxy::stopThread(const StopTransferOptions& options) {

  std::string serviceName = SERVICES_FMS[FILETRANSFERSTOP];

  diet_profile_t* profile = diet_profile_alloc(serviceName, 2);

  //IN Parameters
  diet_string_set(profile,0, msessionKey);

  ::ecorecpp::serializer::serializer _ser;
  string optionsToString =  _ser.serialize_str(const_cast<FMS_Data::StopTransferOptions_ptr>(&options));
  diet_string_set(profile,1, optionsToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);

  return 0;
}



const FMS_Data::FileTransfer& FileTransferProxy::getFileTransfer() const {
  return mtransferInfo;
}

FileTransferProxy::~FileTransferProxy(){

}
