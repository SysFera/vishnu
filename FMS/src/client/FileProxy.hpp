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

/* Definition of the system endianness. */
#define LT_ENDIAN 0
#define BG_ENDIAN 1

/* FileProxy "locality" enumeration. */
typedef enum {
  local,
  remote
} file_host_t;

/* The stat structure differs depending on the system type. */
/* BSD file systems used the same structure for large and small
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

/* Main FileProxy class. Encapsulates all the files attributes. */
/* Defines the common operation on files (local or remote) */
class FileProxy {
private:
  
  std::string mhost;
  mutable bool exist;
  SessionProxy msessionProxy;
  mutable FileStat mfileStat;

protected:

  void setFileStat(const FileStat& fileStat)const;
  void setPath(const std::string& path);
  void setHost(const std::string& host);
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

  virtual int mv(const std::string& dest, const MvFileOptions& options) = 0; 
 
  static std::string extHost(const std::string& path);
  static std::string extName(const std::string& path);
};

/* Utility fonctions */
/* Returns the basename of a file. */
std::string basename(const std::string& path);

std::string gen_uuid();

/* Return the endianness of the system. */
int endianness();
/* Change the endianness of the parameter depending on
 * its type. Generic templated function which works with
 * all scalar data types. */
template <typename T>
T& chgEndianness(T& value) {
  T result;
  char* ptr1 = (char*) &value;
  char* ptr2 = (char*) &result + (sizeof(T)-1);
  for (unsigned short i=0; i<sizeof(T); ++i, ++ptr1, --ptr2) {
    *ptr2=*ptr1;
  }
  value=result;
  return value;
}
/* Change the endianness of the parameter. */
/* If the system is big endian, the endianness is change.
 * Otherwise, the endianness is preserved. */
template <typename T>
T& sysEndianChg(T& value) {
  if (endianness()==BG_ENDIAN)
  return chgEndianness(value);
  return value;
}

#endif
