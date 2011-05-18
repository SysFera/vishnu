#ifndef SSHFILE_HH
#define SSHFILE_HH

#include <string>
#include <utility>

/* The stat command uses different syntax depending on the system type. */
/* BSD and Mac OS X command differs from the Linux one. */
#ifdef BSD_LIKE_SYSTEM
#define STATCMD "stat -f \"%Su %Sg %Lp %u %g %z %a %m %c %HT\" "
#else
#define STATCMD "stat --format=\"%U %G %a %u %g %s %X %Y %Z %F\" "
#endif

/* Define the command to use through ssh. */
#define CHGRPCMD "chgrp "
#define CHMODCMD "chmod "
#define HEADCMD  "head -"
#define MKDIRCMD "mkdir -m "
#define RMCMD    "rm -f "
#define RMDIRCMD "rmdir "
#define TAILCMD  "tail -"
#define CPCMD    "install -m 666 "
#define MVCMD    "mv "
#define LSCMD    "ls -al "
#define LSRECCMD  "ls -Ral "
#define LSSIMPLE "ls -a "

#include "File.hh"

/* A class for file representation through SSH. */
class SSHFile : public File {
private:
  mutable bool upToDate;
  std::string sshHost;
  std::string sshUser;
  std::string sshPublicKey;
  std::string sshPrivateKey;
  std::string sshPassword;
  unsigned int sshPort;
  std::string sshCommand;
  std::string scpCommand;
public:
  SSHFile();
  SSHFile(const std::string& path,
          const std::string& sshHost,
          const std::string& sshUser,
          const std::string& sshPublicKey,
          const std::string& sshPrivateKey,
          const std::string& sshPassword,
          unsigned int sshPort,
          const std::string& sshCommand="/usr/bin/ssh",
          const std::string& scpCommand="/usr/bin/scp");
  SSHFile(const SSHFile& file);
  virtual ~SSHFile();
  
  SSHFile& operator=(const SSHFile& file);
  
  virtual bool isUpToDate() const;
  virtual void getInfos() const;
  
  virtual int chgrp(const std::string& group);
  virtual int chmod(const mode_t mode);
  virtual File* cp(const std::string& dest);
  virtual std::string head(const unsigned int nline);
  virtual int mkdir(const mode_t mode);
  virtual File* mv(const std::string& dest);
  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const unsigned int nline);
  virtual std::list<std::string> lsDir() const;
  virtual std::list<std::string> lsDirRec() const;
  virtual std::list<std::string> lsDirSimple() const;
};
