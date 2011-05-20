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

//UMS Data Headers
#include <UMS_Data.hpp>


// FMS API Headers
#include <api_fms.hpp>

//FMS Data Headers
#include <FMS_Data.hpp>


//FMS client headers
 
#include "FileProxyFactory.hpp"

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
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief change the group of a file
   * \param sessionKey the session key
   * \param path  the file path using host:path format
   * \param group the name of the new group to use for this file
   * \return 0 if everything is OK, another value otherwise
   */
  int
    vishnu::chGrp(const string& sessionKey,const string& path, const string& group)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief  change the permissions of a file
   * \param sessionKey the session key
   * \param path  the file path using host:path format
   * \param options contains the options used to set the new the permission mode  for this file
   * \return 0 if everything is OK, another value otherwise
   */
  int vishnu::chMod(const string& sessionKey,const string& path, const mode_t& mode)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief  copy the file
   * \param sessionKey the session key
   * \param src  the "source" file path using host:path format
   * \param dest  the "destination" file path using host:path format
   * \param options contains the options 
   * \return 0 if everything is OK, another value otherwise
   */
  int vishnu::copyFile(const string& sessionKey,const string& src, const string& dest, const CpFileOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

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
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

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

      SessionProxy sessionProxy(sessionKey);
      FileProxy* f = FileProxyFactory::getFileProxy(sessionProxy,path);

      std::string head;

      head = f->head(options);
      contentOfFile= strdup(head.c_str());

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
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief create a directory
   * \param sessionKey the session key
   * \param path: the directory path using host:path format
   * \return 0 if everything is OK, another value otherwise
   */
  int vishnu::createDir(const string& sessionKey,const string& path)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief move a file
   * \param sessionKey the session key
   * \param src   the "source" file path using host:path format
   * \param dest  the "destination" file path using host:path format
   * \param options   contains the options used to perform the service (like the transfer command :scp or rsync)
   * \return 0 if everything is OK, another value otherwise
   */
  int vishnu::moveFile(const string& sessionKey,const string& src, const string& dest, const MvFileOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

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
      FileTransfer& transferInfo, const MvFileOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /** remove a file
   * \param sessionKey the session key
   * \param path    the file path using host:path format
   \return 0 if everything is OK, another value otherwise
   */
  int vishnu::removeFile(const string& sessionKey,const string& path)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief  remove a directory
   * \param sessionKey the session key
   * \param path    the directory path using host:path format
   * \return 0 if everything is OK, another value otherwise
   */
  int vishnu::removeDir(const string& sessionKey,const string& path)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

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


      SessionProxy sessionProxy(sessionKey);

      FileProxy* f = FileProxyFactory::getFileProxy(sessionProxy,path);

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
  int vishnu::getFilesInfo(const string& sessionKey,const string& path, FileStat& filesInfo)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }

  /**
   * \brief cancel a file transfer
   * \param sessionKey the session key
   * \param options   contains the options used to perform the service (like the transfer id obtained after a call to copyAsyncFile or
   *          moveAsyncFile )
   \return 0 if everything is OK, another value otherwise
   */
  int vishnu::stopFileTransfer(const string& sessionKey,const StopTransferOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }


  /**
   * \brief List  file transfers
   * \param sessionKey the session key
   * \param options contains the options used to perform the service (like the transfer id obtained after a call to copyAsyncFile or
   moveAsyncFile)
   \return 0 if everything is OK, another value otherwise
   */
  int vishnu::listFileTransfers(const string& sessionKey, const LsTransferOptions& options)
    throw (UMSVishnuException, FMSVishnuException, UserException, SystemException){ }






