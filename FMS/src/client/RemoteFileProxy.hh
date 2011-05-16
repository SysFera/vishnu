#ifndef REMOTEFILE_HH
#define REMOTEFILE_HH

#include <string>

#include <sys/types.h>

#include "FileProxy.hh"

/* DIET service name definition macro. */
/* These macro take the host name of the machine as argument. */
#define GETINFOS_SRV(host)   const_cast<char*>(("FileGetInfos"+host).c_str())
#define CHGRP_SRV(host)      const_cast<char*>(("FileChangeGroup"+host).c_str())
#define CHMOD_SRV(host)      const_cast<char*>(("FileChangeMode"+host).c_str())
#define CP_GETID_SRV(host)   const_cast<char*>(("FileCpGetID"+host).c_str())
#define CP_GETFILE_SRV(host) const_cast<char*>(("FileCpGet"+host).c_str())
#define HEAD_SRV(host)       const_cast<char*>(("FileHead"+host).c_str())
#define MKDIR_SRV(host)      const_cast<char*>(("DirectoryMake"+host).c_str())
#define MV_LCL_GET_SRV(host) const_cast<char*>(("FileMvGet"+host).c_str())
#define RM_SRV(host)         const_cast<char*>(("FileRemove"+host).c_str())
#define RMDIR_SRV(host)      const_cast<char*>(("DirectoryRemove"+host).c_str())
#define TAIL_SRV(host)       const_cast<char*>(("FileTail"+host).c_str())
#define LS_SRV(host)         const_cast<char*>(("DirectoryLs"+host).c_str())
#define LSREC_SRV(host)      const_cast<char*>(("DirectoryLsRec"+host).c_str())
#define LSSIMPLE_SRV(host)   const_cast<char*>(("DirectoryLsSimple"+host).c_str())
#define STATUS_SRV(host)     const_cast<char*>(("TransferStatus"+host).c_str())

/* Class for the remote DIET files. */
class RemoteFileProxy : public FileProxy {
private:
  mutable bool upToDate;
  std::string localUser;
  bool printTrID;
public:
  RemoteFileProxy();
  RemoteFileProxy(const SessionProxy& sessionProxy,const std::string& path,
             const std::string& localUser);
  RemoteFileProxy(const RemoteFileProxy& file);
  virtual ~RemoteFileProxy();
  
  virtual bool isUpToDate() const;
  virtual void getInfos() const;
  
  RemoteFileProxy& operator=(const RemoteFileProxy& file);
  
  virtual int chgrp(const std::string& group);
  virtual int chmod(const mode_t mode);
  virtual std::string head(const unsigned int nline);
  virtual int mkdir(const mode_t mode);
  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const unsigned int nline);
  virtual std::list<std::string> lsDir() const;
  virtual std::list<std::string> lsDirRec() const;
  virtual std::list<std::string> lsDirSimple() const;
  
  void printTransferID(const bool printTrID);
};

#endif
