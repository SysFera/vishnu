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
  LocalFile(const SessionServer& sessionServer,const std::string& path);
  LocalFile(const LocalFile& file);
  
  LocalFile& operator=(const LocalFile& file);
  
  virtual bool isUpToDate() const;
  virtual void getInfos() const;
  
  virtual int chgrp(const std::string& group);
  virtual int chmod(const mode_t mode);
  virtual std::string head(const unsigned int nline);
  virtual std::string getContent();
  virtual int mkfile(const mode_t mode);
  virtual int mkdir(const mode_t mode);
  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const unsigned int nline); 
  virtual std::list<std::string> ls(const FMS_Data::LsDirOptions& options) const;

  virtual int cp(const std::string& path, const FMS_Data::CpFileOptions& options);
};

#endif
