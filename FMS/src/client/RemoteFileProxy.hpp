/**
 * \file RemoteFileProxy.hpp
 * This file declares a  proxy class to handle a a remote file
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */




#ifndef REMOTEFILE_HH
#define REMOTEFILE_HH

#include <string>

#include <sys/types.h>

#include "FileProxy.hpp"




/**
 * \brief  Class for the remote  files.
 */


class RemoteFileProxy : public FileProxy {

public:

  /**
     * \brief The default constructor
     */
  RemoteFileProxy();
  /**
     * \brief Another constructor taking two parameters
     * \param sessionProxy  the session object proxy
     * \param path the path of the file
     * \param localUser the local user of the file
     */
  RemoteFileProxy(const SessionProxy& sessionProxy,const std::string& path);
  /**
     * \brief Another constructor by reference
     * \param file the other file
     */
  explicit  RemoteFileProxy(const RemoteFileProxy& file);
  /**
 * \brief The default destructor
 */
  virtual ~RemoteFileProxy();

  /**
     * \brief To check if the file information is up to date
     * \return true if the file is already updated, false otherwise
     */
  virtual bool isUpToDate() const;
  /**
     * \brief To get the file inode information
     */
  virtual void getInfos() const;

  /**
     * \brief The assignment operator
     * \param file the object to assign
     * \return the new assigned file
     */
  RemoteFileProxy& operator=(const RemoteFileProxy& file);

  /**
     * \brief To update the group of the file
     * \param group the new group
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int chgrp(const std::string& group);
  /**
     * \brief To update the new file access permissions
     * \param mode the new file access permissions
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int chmod(const mode_t mode);
  /**
     * \brief To get the first lines of the file
     * \param options the options object
     * \return the first lines of the file
     */
  virtual std::string head(const FMS_Data::HeadOfFileOptions& options);
  /**
     * \brief To get the last lines of the file
     * \param options the options object
     * \return the last lines of the file
     */
  virtual std::string tail(const FMS_Data::TailOfFileOptions& options);
  /**
     * \brief To get the content of the file
     * \return the content of the file
     */
  virtual std::string getContent();
  /**
     * \brief To create a new file
     * \param mode the access permission of the file
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int mkfile(const mode_t mode=defaultFileAccessMode);
  /**
     * \brief To create a new directory
     * \param options the directory creation options
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int mkdir(const FMS_Data::CreateDirOptions& options);

  /**
     * \brief To remove a file
     * \param options the remove file options
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int rm(const FMS_Data::RmFileOptions& options);
  /**
     * \brief To remove an empty directory
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int rmdir();
  /**
     * \brief To list the content of a directory
     * \param options the list options
     * \return the content of the directory
     */
  //   virtual std::list<std::string> ls(const LsDirOptions& options) const;
  virtual FMS_Data::DirEntryList* ls(const FMS_Data::LsDirOptions& options) const ;
  /**
     * \brief To copy the file
     * \param dest the copy destination
     * \param options the copy options
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int cp(const std::string& dest, const FMS_Data::CpFileOptions& options);

  /**
     * \brief To move the file
     * \param dest the move destination
     * \param options the move options
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int mv(const std::string& dest, const FMS_Data::CpFileOptions& options);

  /**
     * \brief To copy the file in asynchronous mode
     * \param dest the copy destination
     * \param options the copy options
     * \param fileTransfer information about the transfer like its identifier
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int cpAsync(const std::string& dest, const FMS_Data::CpFileOptions& options, FMS_Data::FileTransfer& fileTransfer);

  /**
     * \brief To move the file in asynchronous mode
     * \param dest the move destination
     * \param options the move options
     * \param fileTransfer information about the transfer like its identifier
     * \return 0 if the command succeeds, an error code otherwise
     */
  virtual int mvAsync(const std::string& dest, const FMS_Data::CpFileOptions& options, FMS_Data::FileTransfer& fileTransfer);

private:

  /**
     * \brief A flag to store the file information state
     */
  mutable bool upToDate;

  /**
     * \brief A generic class to handle a local to remote file transfer
     * \param dest the destination
     * \param options the transfer options
     * \param serviceName the name of the service
     * \param fileTransfer information about the transfer
     * \return 0 if the function succeeds or an error code otherwise
     */
  int transferFile(const std::string& dest,
                   const FMS_Data::CpFileOptions& options,
                   const std::string& serviceName,
                   FMS_Data::FileTransfer& fileTransfer);
};

#endif
