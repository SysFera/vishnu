#ifndef LOCALFILEPROXY_HH
#define LOCALFILEPROXY_HH

#include <string>


#include "SessionProxy.hpp"

#include "FileProxy.hpp"

/* Local implementation of a DIET file. */

class LocalFileProxy : public FileProxy {
private:
  mutable bool upToDate;
public:
  LocalFileProxy();
  LocalFileProxy(const SessionProxy& sessionProxy,const std::string& path);
  LocalFileProxy(const LocalFileProxy& file);
  
  LocalFileProxy& operator=(const LocalFileProxy& file);
  
  virtual bool isUpToDate() const;
  virtual void getInfos() const;
  
  virtual int chgrp(const std::string& group);
  virtual int chmod(const mode_t mode);
  virtual std::string head(const HeadOfFileOptions& options);
  virtual std::string getContent() ;
  virtual int mkfile(const mode_t mode=420);
  virtual int mkdir(const mode_t mode=493) ;
  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const TailOfFileOptions& options); 
<<<<<<< HEAD
  virtual int cp(const std::string& dest, const CpFileOptions& options);
=======
  //virtual int cp(const std::string& dest, const CpFileOptions& options);
>>>>>>> 1ab0f1541d6cb7717e02f9b470d3655507485bf6
 // virtual int mv(const std::string& dest);

  virtual std::list<std::string> ls(const LsDirOptions& options) const;
};

#endif
