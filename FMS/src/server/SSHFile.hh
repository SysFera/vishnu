#ifndef SSHFILE_HH
#define SSHFILE_HH

#include <string>
#include <utility>
#include "SessionServer.hpp"

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
#define CATCMD  "cat "
#define MKDIRCMD "mkdir -m "
#define MKFILECMD "touch "
#define RMCMD    "rm -f "
#define RMDIRCMD "rmdir "
#define TAILCMD  "tail -"
#define CPCMD    "scp -o Compression=yes -o StrictHostKeyChecking=no "
#define MVCMD    "mv "
#define LSCMD    "ls  "
#define LSACMD  "ls -a "
#define LSLCMD "ls -l "
#define LSALCMD  "ls -al "

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
  mutable std::string merror;
public:
  SSHFile();
  SSHFile(const SessionServer& sessionServer,
          const std::string& path,
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
  std::string getErrorMsg() const;

  virtual int chgrp(const std::string& group);
  virtual int chmod(const mode_t mode);
  virtual std::string head(const HeadOfFileOptions& options);
  virtual std::string getContent();
  virtual int mkfile(const mode_t mode);
  virtual int mkdir(const mode_t mode);
  virtual int rm();
  virtual int rmdir();
  virtual std::string tail(const TailOfFileOptions& options);
  virtual std::list<std::string> ls(const LsDirOptions& options) const;
  virtual int cp(const std::string& path, const CpFileOptions& options);
  virtual int mv(const std::string& path, const CpFileOptions& options);
 
};
/* A class to call command through SSH. */
class SSHExec {
private:
  std::string sshCommand;
  std::string scpCommand;
  std::string server;
  unsigned int sshPort;
  std::string userName;
  std::string password;
  std::string publicKey;
  std::string privateKey;
  mutable int lastExecStatus;
public:
  SSHExec(const std::string& sshCommand,
          const std::string& scpCommand,
          const std::string& server, unsigned int sshPort,
          const std::string& userName,
          const std::string& password, const std::string& publicKey,
          const std::string& privateKey);
  
  const int& getLastExecStatus() const;
  
  std::pair<std::string, std::string> exec(const std::string& cmd) const;
  void copyFrom(const std::string& file, const std::string& src) const;
  void copyTo(const std::string& file, const std::string& dest) const;
};

#endif
