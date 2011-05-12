/**
 * \file SSHExec.hpp
 * \brief This file implements the SSH execution
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date 11/05/2011
 */
#ifndef __SSHEXEC__HH__
#define __SSHEXEC__HH__

#include <string>
#include <utility>

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

  SSHResult* exec(const std::string& cmd) const;
};

#endif // end sshexec
