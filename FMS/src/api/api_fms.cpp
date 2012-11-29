/** \file api_fms.cpp
 * \brief This file contains the VISHNU api functions for FMS package.
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \date MAY 2011
 */

// C Headers
#include <cstring>

// C++ Headers
#include <string>
#include <stdexcept>
#include <pthread.h>


//Boost Headers
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

//UMS Data Headers
#include <UMS_Data.hpp>

//Vishnu Headers
#include "QueryProxy.hpp"
#include "UtilsProxy.hpp"
#include "utilVishnu.hpp"


// FMS API Headers
#include <api_fms.hpp>


//FMS client headers

#include "FileProxyFactory.hpp"
#include "FileTransferProxy.hpp"
using namespace FMS_Data;
using namespace UMS_Data;
using namespace std;

/**
 * \brief create a new remote file
 * \param sessionKey the session key
 * \param path  the file path using host:path format
 * \return 0 if everything is OK, another value otherwise
 */
  int
vishnu::touch(const string& sessionKey,const string& path)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));


    int result= f->mkfile();


    return result;


  }
/**
 * \brief create a directory
 * \param sessionKey the session key
 * \param path: the directory path using host:path format
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::mkdir(const string& sessionKey,const string& path,const CreateDirOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));


    int result= f->mkdir(options);


    return result;



  }
/** remove a file
 * \param sessionKey the session key
 * \param path    the file path using host:path format
 * \param options contains options used to perform the remove file function
 \return 0 if everything is OK, another value otherwise
 */
int vishnu::rm(const string& sessionKey,const string& path,const RmFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    int result= f->rm(options);

    return result;

  }

/**
 * \brief  remove a directory
 * \param sessionKey the session key
 * \param path    the directory path using host:path format
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::rmdir(const string& sessionKey,const string& path)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));


    int result= f->rmdir();


    return result;




  }



/**
 * \brief change the group of a file
 * \param sessionKey the session key
 * \param group the name of the new group to use for this file
 * \param path  the file path using host:path format
 * \return 0 if everything is OK, another value otherwise
 */
  int
vishnu::chgrp(const string& sessionKey, const string& group, const string& path)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));


    int result= f->chgrp(group);

    return result;


  }

/**
 * \brief  change the permissions of a file
 * \param sessionKey the session key
 * \param mode the path new mode
 * \param path  the file path using host:path format
 * \param options contains the options used to set the new the permission mode  for this file
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::chmod(const string& sessionKey, const mode_t& mode, const string& path)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    int result= f->chmod(mode);

    return result;

  }

/**
 * \brief  copy the file
 * \param sessionKey the session key
 * \param src  the "source" file path using host:path format
 * \param dest  the "destination" file path using host:path format
 * \param options contains the options
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::cp(const string& sessionKey,const string& src, const string& dest, const CpFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){
  int result = 0;
    if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer command type: its value must be 0 (scp) or 1 (rsync)");
    }
    FMSVishnuException e(ERRCODE_RUNTIME_ERROR, "Unknwon copy error");

    try{
      FileTransferProxy fileTransferProxy(sessionKey, src, dest);
      result = fileTransferProxy.addCpThread(options);
      return result;
    } catch (FMSVishnuException& ex){
      e.appendMsgComp(" "+src+": "+ex.what());
    }


// If source is localhost
    if ((src.find(std::string("localhost"))!=std::string::npos)){
      // Get all the IP for the machine

      std::vector<std::string> list = getIPList();
// Try the transfert for each IP
        for (unsigned int i = 0; i < list.size(); i++) {
          std::string tmp = src;
          setIP(tmp, list.at(i));
          try{
            FileTransferProxy fileTransferProxy(sessionKey, tmp, dest);
            result = fileTransferProxy.addCpThread(options);
          } catch (VishnuException& ex){
            e.appendMsgComp(" "+list.at(i)+": "+ex.what());
	    continue;
          }
// The function returns '0' in case of success
          if (result==0){
            return result;
          }
        }// Else use the given IP
// Else if destination is localhost
    } else if ((dest.find(std::string("localhost"))!=std::string::npos)) {
      // Get all the IP for the machine
      std::vector<std::string> list = getIPList();
// Try the transfert for each IP
      for (unsigned int i =0; i < list.size(); i++){
        std::string tmp = dest;
        setIP(tmp, list.at(i));
        try{
          FileTransferProxy fileTransferProxy(sessionKey, src, tmp);
          result = fileTransferProxy.addCpThread(options);
        } catch (VishnuException& ex){
          e.appendMsgComp(" "+list.at(i)+": "+ex.what());
	  continue;
        }

// The function returns '0' in case of success
        if (result==0){

          return result;
        }
      }// Else use the given IP

    }

    throw e;
    return result;
}


/**
 * \brief copy the file in a asynchronous mode
 * \param sessionKey the session key
 * \param src   the "source" file path using host:path format
 * \param dest  the "destination" file path using host:path format
 * \param transferInfo contains different information about the submitted file
 * transfer (like the transfer identifier)
 * \param options contains options used to perform the file transfer
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::acp(const string& sessionKey,const string& src, const string& dest,
    FileTransfer& transferInfo, const CpFileOptions& options)
throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

  if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer commad type: its value must be 0 (scp) or 1 (rsync)");
  }
  FileTransferProxy fileTransferProxy(sessionKey, src, dest);
  int result = fileTransferProxy.addCpAsyncThread(options);
  transferInfo = fileTransferProxy.getFileTransfer();
  return result;
}

/**
 * \brief get the first lines of a file
 * \param sessionKey the session key
 * \param path   the file path using host:path format
 * \param contentOfFile  the first "nline" lines of the file
 * \param options   contains the options used to perform the service (like the maximum number of lines to get)
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::head(const string& sessionKey,const string& path, string& contentOfFile, const HeadOfFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);
    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    contentOfFile = f->head(options);

    return 0;

  }
/**
 * \brief get the content of a file
 * \param sessionKey the session key
 * \param path   the file path using host:path format
 * \param contentOfFile  the content of specified the file
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::more(const string& sessionKey,const string& path, string& contentOfFile)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    contentOfFile= f->getContent();

    return 0;

  }

/**
 * \brief get the list of files and subdirectories of a directory
 * \param sessionKey the session key
 * \param path  the directory path using host:path format
 * \param dirContent  the directory content
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::ls(const string& sessionKey,const string& path, DirEntryList& dirContent,const LsDirOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    // initialize the list of dirEntries
    dirContent.getDirEntries().clear();


    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    FMS_Data::DirEntryList* dirContent_ptr = f->ls(options);

    if(dirContent_ptr != NULL) {
      FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
      for(unsigned int i = 0; i < dirContent_ptr->getDirEntries().size(); i++) {
        FMS_Data::DirEntry_ptr dirEntry = ecoreFactory->createDirEntry();

        //To copy the content and not the pointer
        *dirEntry = *dirContent_ptr->getDirEntries().get(i);
        dirContent.getDirEntries().push_back(dirEntry);
      }
      delete dirContent_ptr;
    }
    return 0;
  }

/**
   * \brief move a file in synchronous mode
   * \param sessionKey the session key
   * \param src:   the "source" file path using host:path format
   * \param dest:  the "destination" file path using host:path format
   * \param transferInfo contains different information about the submitted file
   * transfer (like the transfer identifier)
   * \param options   contains the options used to perform the service (like the transfer command :scp or rsync)
   * \return 0 if everything is OK, another value otherwise
   */
int vishnu::mv(const string& sessionKey,const string& src, const string& dest,const CpFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

  if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer commad type: its value must be 0 (scp) or 1 (rsync)");
  }
  int result = 0;
  FMSVishnuException e(ERRCODE_RUNTIME_ERROR, "Unknwon move error");
  try{
    FileTransferProxy fileTransferProxy(sessionKey, src, dest);
    result = fileTransferProxy.addMvThread(options);
    return result;
  } catch (FMSVishnuException& ex){
    e.appendMsgComp(" "+src+": "+ex.what());
  }


// If source is localhost
  if ((src.find(std::string("localhost"))!=std::string::npos)){
    // Get all the IP for the machine
    std::vector<std::string> list = getIPList();
// Try the transfert for each IP
    for (unsigned int i = 0; i < list.size(); i++){
      std::string tmp = src;
      setIP(tmp, list.at(i));
      try{
        FileTransferProxy fileTransferProxy(sessionKey, tmp, dest);
        result = fileTransferProxy.addMvThread(options);
      } catch (VishnuException& ex){
        e.appendMsgComp(" "+list.at(i)+": "+ex.what());
	continue;
      }
// The function returns '0' in case of success
      if (result==0){
        return result;
      }
    }// Else use the given IP
// Else if destination is localhost
  } else if ((dest.find(std::string("localhost"))!=std::string::npos)) {
    // Get all the IP for the machine
    std::vector<std::string> list = getIPList();
// Try the transfert for each IP
    for (unsigned int i = 0; i < list.size(); i++){
      std::string tmp = dest;
      setIP(tmp, list.at(i));
      try{
        FileTransferProxy fileTransferProxy(sessionKey, src, tmp);
        result = fileTransferProxy.addMvThread(options);
      } catch (VishnuException& ex){
        e.appendMsgComp(" "+list.at(i)+": "+ex.what());
	continue;
      }

// The function returns '0' in case of success
      if (result==0){
        return result;
      }
    }// Else use the given IP

  }
  throw e;
  return result;

}
/**
 * \brief move a file in a asynchronous mode
 * \param sessionKey the session key
 * \param src:   the "source" file path using host:path format
 * \param dest:  the "destination" file path using host:path format
 * \param transferInfo contains different information about the submitted file
 * transfer (like the transfer identifier)
 * \param options   contains the options used to perform the service (like the transfer command :scp or rsync)
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::amv(const string& sessionKey,const string& src, const string& dest,
    FileTransfer& transferInfo, const CpFileOptions& options)
throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

  if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
    throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer commad type: its value must be 0 (scp) or 1 (rsync)");
  }

  FileTransferProxy fileTransferProxy(sessionKey, src, dest);
  int result = fileTransferProxy.addMvAsyncThread(options);
  transferInfo = fileTransferProxy.getFileTransfer();
  return result;
}

/**
 * \brief get the last lines of a file
 * \param sessionKey the session key
 * \param path    the file path using host:path format
 * \param contentOfFile  the last "nline" lines of the file
 * \param options  the options used to perform the service
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::tail(const string& sessionKey,const string& path, string& contentOfFile,const TailOfFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f(FileProxyFactory::getFileProxy(sessionProxy,path));

    contentOfFile= f->tail(options);

    return 0;

  }

/**
 * \brief  obtain informations about a file
 * \param sessionKey the session key
 * \param path the file path using host:path format
 * \param :  a buffer to store the informations
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::stat(const string& sessionKey,const string& path, FileStat& fileInfos)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    FileProxy* f = FileProxyFactory::getFileProxy(sessionProxy,path);

    f->getInfos();

    fileInfos=f->getFileStat();

    return 0;

  }

/**
 * \brief cancel a file transfer
 * \param sessionKey the session key
 * \param options   contains the options used to perform the service (like the transfer id obtained after a call to acp or
 *          amv )
 \return 0 if everything is OK, another value otherwise
 */
int vishnu::stopFileTransfer(const string& sessionKey,const StopTransferOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    StopTransferOptions optionsCompleted(options);

    FileTransferProxy fileTransferProxy(sessionKey);

    int result = fileTransferProxy.stopThread(optionsCompleted);

    return result;


  }


/**
 * \brief List  file transfers
 * \param sessionKey the session key
 * \param fileTransferList the file transfer list
 * \param options contains the options used to perform the service (like the transfer id obtained after a call to acp or
 amv)
 \return 0 if everything is OK, another value otherwise
 */
int vishnu::listFileTransfers(const string& sessionKey,FileTransferList& fileTransferList, const LsTransferOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    // initialize the list of file transfers
    fileTransferList.getFileTransfers().clear();

    if(options.getStatus() < 0 || options.getStatus() > 4) {
      throw UserException(ERRCODE_INVALID_PARAM, "The file transfer status option value is incorrect");
    }

    std::string serviceName = "FileTransfersList";

    SessionProxy sessionProxy(sessionKey);

    QueryProxy<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList>
      query(options, sessionProxy, serviceName);

    FMS_Data::FileTransferList* listFileTransfers_ptr = query.list();

    if(listFileTransfers_ptr != NULL) {
      FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
      for(unsigned int i = 0; i < listFileTransfers_ptr->getFileTransfers().size(); i++) {
        FMS_Data::FileTransfer_ptr fileTransfer = ecoreFactory->createFileTransfer();

        //To copy the content and not the pointer
        *fileTransfer = *listFileTransfers_ptr->getFileTransfers().get(i);
        fileTransferList.getFileTransfers().push_back(fileTransfer);
      }
      delete listFileTransfers_ptr;
    }
    return 0;

  }
