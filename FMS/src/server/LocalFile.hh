#ifndef LOCALFILE_HH
#define LOCALFILE_HH

#include <string>

#include "File.hh"

/* Local implementation of a DIET file. */
class LocalFile : public File {
private:
  mutable bool upToDate;
public:
  LocalFile();
  LocalFile(const std::string& path);
  LocalFile(const LocalFile& file);
  
  LocalFile& operator=(const LocalFile& file);
  
  virtual bool isUpToDate() const;
  virtual void getInfos() const;
  
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
};

#endif
