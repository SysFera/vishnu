/**
 * \file SSHExec.hpp
 * \brief This file implements call the ssh exec implementation to call the command throug ssh (or not depending on the implementation)
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date 11/05/2011
 */
#ifndef __SSHEXEC__HH__
#define __SSHEXEC__HH__


#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "SSHResult.hpp"
class SSHExec;
#include "SSHExecImpl.hpp"

/**
 * \class SSHExec
 * \brief A class to call command through SSH.
 */
class SSHExec {
public:
  /**
   * \brief Constructor
   * \param sshCommand The ssh command to use
   * \param scpCommand The scp command to use
   * \param The distant machine to execute the command
   * \param sshPort The port to use
   * \param userName The username to log in
   * \param password The password to log in
   * \param publicKey The public key for the ssh connexion
   * \param privateKey The privateKey for the ssh connexion
   * \param The functor to use
   */
  SSHExec(const std::string& sshCommand,
          const std::string& scpCommand,
          const std::string& server, unsigned int sshPort,
          const std::string& userName,
          const std::string& password, const std::string& publicKey,
          const std::string& privateKey,
	  boost::function1<SSHResult*, string>& func);

  /**
   * \brief Function to execute the command
   *
   */
  SSHResult*
  exec(const std::string& cmd) const;
  /**
   * \brief To get the ssh command
   */
  string
  getSSHcmd();
  /**
   * \brief To get the private key
   */
  string
  getPrivateKey();
  /**
   * \brief To get the username
   */
  string
  getUsername();
  /**
   * \brief To get the ssh port
   */
  int
  getSSHPort();
  /**
   * \brief To get the server
   */
  string
  getServer();
private:
  /**
   * \brief The ssh command to use -> /usr/bin/ssh for instance
   */
  std::string  msshCommand;
  /**
   * \brief The scp command to use
   */
  std::string  mscpCommand;
  /**
   * \brief The server to execute the command over
   */
  std::string  mserver;
  /**
   * \brief The port to use
   */
  unsigned int msshPort;
  /**
   * \brief The username to use
   */
  std::string  muserName;
  /**
   * \brief The password to use
   */
  std::string  mpassword;
  /**
   * \brief The public key to use
   */
  std::string  mpublicKey;
  /**
   * \brief The private key to use
   */
  std::string  mprivateKey;
  /**
   * \brief The class containing the implementations of the exec function
   */
  SSHExecImpl*  mexec;
  /**
   * \brief The functor
   */
  boost::function1<SSHResult*, string> mfunc;

}; // end class ssh exec

#endif // end sshexec
