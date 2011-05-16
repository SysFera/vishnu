#ifndef FILE_HH
#define FILE_HH

#include <string>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>

/* Definition of the system endianness. */
#define LT_ENDIAN 0
#define BG_ENDIAN 1

/* File type enumeration. */
typedef enum {
  block,
  character,
  directory,
  symboliclink,
  sckt,
  fifo,
  regular
} file_type_t;

/* File "locality" enumeration. */
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

/* Main File class. Encapsulates all the files attributes. */
/* Defines the common operation on files (local or remote) */
class File {
private:
  std::string path;
  std::string host;
  mutable std::string owner;
  mutable std::string group;
  mutable mode_t perms;
  mutable uid_t uid;
  mutable gid_t gid;
  mutable file_size_t size;
  mutable time_t atime;
  mutable time_t mtime;
  mutable time_t ctime;
  mutable file_type_t type;
  mutable bool exist;
protected:
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
  void setType(const file_type_t type) const;
  void exists(const bool exist) const;
public:
  File();
  File(const std::string& path);
  File(const File& file);
  virtual ~File();
  
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
  file_type_t getType() const;
  file_host_t getHostType() const;
  bool exists() const;

  virtual bool isUpToDate() const = 0;
  virtual void getInfos() const = 0;
  
  File& operator=(const File& file);
  bool operator==(const File& file);
  bool operator<(const File& file);
  
  virtual int chgrp(const std::string& group) = 0;
  virtual int chmod(const mode_t mode) = 0;
  virtual std::string head(const unsigned int nline) = 0;
  virtual int mkdir(const mode_t mode) = 0;
  virtual int rm() = 0;
  virtual int rmdir() = 0;
  virtual std::string tail(const unsigned int nline) = 0;
  virtual std::list<std::string> lsDir() const = 0;
  virtual std::list<std::string> lsDirRec() const = 0;
  virtual std::list<std::string> lsDirSimple() const = 0;
  
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
