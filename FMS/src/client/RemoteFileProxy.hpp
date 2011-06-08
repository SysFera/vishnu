#ifndef REMOTEFILE_HH
#define REMOTEFILE_HH

#include <string>

#include <sys/types.h>

#include "FileProxy.hpp"




/* Class for the remote DIET files. */


class RemoteFileProxy : public FileProxy {
private:
  mutable bool upToDate;
  std::string localUser;
  bool printTrID;

  template <class TypeOfOption>
    int transferFile(const std::string& dest,
                     const TypeOfOption& options,
                     const std::string& serviceName,
                     FileTransfer& fileTransfer); 

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
  virtual std::string head(const HeadOfFileOptions& options);
  virtual std::string getContent();
  virtual int mkfile(const mode_t mode=420);
  virtual int mkdir(const mode_t mode=493);

  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const TailOfFileOptions& options);
  virtual std::list<std::string> ls(const LsDirOptions& options) const; 
  void printTransferID(const bool printTrID);

  virtual int cp(const std::string& dest, const CpFileOptions& options);

  virtual int mv(const std::string& dest, const CpFileOptions& options);

  virtual int cpAsync(const std::string& dest, const CpFileOptions& options, FileTransfer& fileTransfer);

  virtual int mvAsync(const std::string& dest, const CpFileOptions& options, FileTransfer& fileTransfer);

};

#endif
