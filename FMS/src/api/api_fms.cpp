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
vishnu::createFile(const string& sessionKey,const string& path)
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
int vishnu::createDir(const string& sessionKey,const string& path)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));


    int result= f->mkdir();


    return result;



  }
/** remove a file
 * \param sessionKey the session key
 * \param path    the file path using host:path format
 * \param options contains options used to perform the remove file function
 \return 0 if everything is OK, another value otherwise
 */
int vishnu::removeFile(const string& sessionKey,const string& path,const RmFileOptions& options)
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
int vishnu::removeDir(const string& sessionKey,const string& path)
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
vishnu::chGrp(const string& sessionKey, const string& group, const string& path)
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
int vishnu::chMod(const string& sessionKey, const mode_t& mode, const string& path)
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
int vishnu::copyFile(const string& sessionKey,const string& src, const string& dest, const CpFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer commad type: its value must be 0 (scp) or 1 (rsync)");
    }

    FileTransferProxy fileTransferProxy(sessionKey, src, dest);
    int result = fileTransferProxy.addCpThread(options);
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
int vishnu::copyAsyncFile(const string& sessionKey,const string& src, const string& dest,
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
int vishnu::headOfFile(const string& sessionKey,const string& path, string& contentOfFile, const HeadOfFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);
    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    std::string head;

    head = f->head(options);

    contentOfFile= strdup(head.c_str());

    return 0;

  }
/**
 * \brief get the content of a file
 * \param sessionKey the session key
 * \param path   the file path using host:path format
 * \param contentOfFile  the content of specified the file
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::contentOfFile(const string& sessionKey,const string& path, string& contentOfFile)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    std::string content= f->getContent();

    contentOfFile= strdup(content.c_str());

    return 0;

  }

/**
 * \brief get the list of files and subdirectories of a directory
 * \param sessionKey the session key
 * \param path  the directory path using host:path format
 * \param dirContent  the directory content
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::listDir(const string& sessionKey,const string& path, StringList& dirContent,const LsDirOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f (FileProxyFactory::getFileProxy(sessionProxy,path));

    std::list<string> tmpList(f->ls(options));

    std::list<string>::const_iterator it;

    for (it =tmpList.begin();it!=tmpList.end();++it ){

      dirContent.addStrings(*it);

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
  int vishnu::moveFile(const string& sessionKey,const string& src, const string& dest,const CpFileOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

      if((options.getTrCommand() < 0) || options.getTrCommand() > 2) {
        throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer commad type: its value must be 0 (scp) or 1 (rsync)");
      }

      FileTransferProxy fileTransferProxy(sessionKey, src, dest);
      int result = fileTransferProxy.addMvThread(options);
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
int vishnu::moveAsyncFile(const string& sessionKey,const string& src, const string& dest,
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
 * \param contentOfFile  an user-allocated buffer to store the result
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::tailOfFile(const string& sessionKey,const string& path, string& contentOfFile,const TailOfFileOptions& options)
  throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){

    //To check the remote path
    vishnu::checkRemotePath(path);

    SessionProxy sessionProxy(sessionKey);

    boost::scoped_ptr<FileProxy> f(FileProxyFactory::getFileProxy(sessionProxy,path));

    std::string tail;

    tail= f->tail(options);

    contentOfFile= strdup(tail.c_str());

    return 0;

  }

/**
 * \brief  obtain informations about a file
 * \param sessionKey the session key
 * \param path the file path using host:path format
 * \param :  a buffer to store the informations
 * \return 0 if everything is OK, another value otherwise
 */
int vishnu::getFilesInfo(const string& sessionKey,const string& path, FileStat& fileInfos)
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
 * \param options   contains the options used to perform the service (like the transfer id obtained after a call to copyAsyncFile or
 *          moveAsyncFile )
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
 * \param options contains the options used to perform the service (like the transfer id obtained after a call to copyAsyncFile or
 moveAsyncFile)
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
