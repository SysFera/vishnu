#ifndef FILEPROXY_HH
#define FILEPROXY_HH

#include <string>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>

#include "SessionProxy.hpp"
#include "FMS_Data_forward.hpp"
#include "FMS_Data.hpp"

using namespace FMS_Data;


/**
 * \brief FileProxy "locality" enumeration.
 */
typedef enum {
  local,
  remote
} file_host_t;

/**
 * \brief The stat structure differs depending on the system type. 
 * BSD file systems used the same structure for large and small
 * partition type.
 */
#if FS_32 | BSD_LIKE_SYSTEM
typedef struct stat file_stat_t;
typedef off_t file_size_t;
#define STAT(path, buf) stat((path), (buf))
#else
typedef struct stat64 file_stat_t;
typedef off64_t file_size_t;
#define STAT(path, buf) stat64((path), (buf))
#endif

/**
 * \brief Main FileProxy class. Encapsulates all the files attributes. 
 *  Defines the common operation on files (local or remote)
 */
class FileProxy {

  private:
    /**
     * \brief The host of the file
     */
    std::string mhost;
  
    /**
     * \brief To check if the file exists
     */  
    mutable bool exist;
    
    /**
     * \brief The session proxy object
     */  
      SessionProxy msessionProxy;
    /**
     * \brief The inode information of the file
     */  
      mutable FileStat mfileStat;

    protected:

    /**
     * \brief To update the inode information of the file
     * \param fileStat the new inode information
     */  
      void setFileStat(const FileStat& fileStat)const;
    /**
     * \brief To update the host  of the file
     * \param fileStat the new host
     */  
      void setPath(const std::string& path);
   
      /**
     * \brief To update the host  of the file
     * \param fileStat the new host
     */  
     
      void setHost(const std::string& host);
    
      /**
     * \brief To update the host  of the file
     * \param fileStat the new host
     */  
      void setOwner(const std::string& owner) const;
      void setGroup(const std::string& group) const;
      void setPerms(const mode_t perms) const;
      void setUid(const uid_t uid) const;
      void setGid(const gid_t gid) const;
      void setSize(const file_size_t size) const;
      void setAtime(const time_t atime) const;
      void setMtime(const time_t mtime) const;
      void setCtime(const time_t ctime) const;
      void setType(const FileType& type) const;
      void exists(const bool exist) const;
    public:
 
  FileProxy();
  
  FileProxy(const SessionProxy& sessionProxy,
            const std::string& path);

  FileProxy(const FileProxy& file);
  virtual ~FileProxy();
  
  const SessionProxy& getSession() const;
  const FileStat& getFileStat() const;
  
  const std::string& getPath() const;
  const std::string& getOwner() const;
  const std::string& getGroup() const;
  const std::string& getHost() const;
  mode_t getPerms() const;
  uid_t getUid() const;
  gid_t getGid() const;
  file_size_t getSize() const;
  time_t getAtime() const;
  time_t getMtime() const;
  time_t getCtime() const;
  FileType getType() const;
  file_host_t getHostType() const;
  bool exists() const;

  virtual bool isUpToDate() const = 0;
  virtual void getInfos() const = 0;
  
  FileProxy& operator=(const FileProxy& file);
  bool operator==(const FileProxy& file);
  bool operator<(const FileProxy& file);
  
  virtual int chgrp(const std::string& group) = 0;
  virtual int chmod(const mode_t mode) = 0;
  virtual std::string head(const HeadOfFileOptions& options) = 0;
  virtual std::string getContent() = 0;
  virtual int mkfile(const mode_t mode=420) = 0; //FIXME use options 
  virtual int mkdir(const mode_t mode=493) = 0;//FIXME use options
  virtual int rm() = 0;
  virtual int rmdir() = 0;
  virtual std::string tail(const TailOfFileOptions& options) = 0;
  virtual std::list<std::string> ls(const LsDirOptions& options) const = 0; 
  
  virtual int cp(const std::string& dest, const CpFileOptions& options) = 0;

  virtual int mv(const std::string& dest, const CpFileOptions& options) = 0; 

  virtual int cpAsync(const std::string& dest, 
                      const CpFileOptions& options, 
                      FileTransfer& fileTransfer) = 0;
  
  virtual int mvAsync(const std::string& dest, 
                      const CpFileOptions& options, 
                      FileTransfer& fileTransfer) = 0;


  static std::string extHost(const std::string& path);
  static std::string extName(const std::string& path);
};



#endif
