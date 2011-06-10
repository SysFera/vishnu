#include <boost/scoped_ptr.hpp>

#include "FileTransferProxy.hpp"
#include "FileProxyFactory.hpp"
#include "SessionProxy.hpp"
#include "utilClient.hpp"
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

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->cpAsync(mdestFilePath,options,mtransferInfo);

  return result;

}

int FileTransferProxy::addMvAsyncThread(const CpFileOptions& options){

  SessionProxy sessionProxy(msessionKey);

  boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,msrcFilePath));

  int result= f->mvAsync(mdestFilePath,options,mtransferInfo);

  return result;
}



int FileTransferProxy::stopThread(const StopTransferOptions& options) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* optionsToString = NULL;
  char* errorInfo = NULL;
  std::string serviceName = "FileTransferStop";

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(msessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+msessionKey;
    raiseDietMsgException(msgErrorDiet);
  }


  ::ecorecpp::serializer::serializer _ser;
  //To serialize the option object in to optionsInString
  optionsToString =  strdup(_ser.serialize_str(const_cast<FMS_Data::StopTransferOptions_ptr>(&options)).c_str());

  if (diet_string_set(diet_parameter(profile,1), optionsToString, DIET_VOLATILE)) {
    msgErrorDiet += "with jobInString parameter "+std::string(optionsToString);
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
  diet_profile_free(profile);
  return 0;

}



const FMS_Data::FileTransfer& FileTransferProxy::getFileTransfer() const {
  return mtransferInfo;
}

FileTransferProxy::~FileTransferProxy(){

}
