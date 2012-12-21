/**
 * \file FileFactory.hpp
 * This file contains a  file server class factory declarartion
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#ifndef FILEFACTORY_HH
#define FILEFACTORY_HH

#include <string>

#include "SessionServer.hpp"
#include "File.hpp"
#include "SSHFile.hpp"

/**
 * \brief  A factory class for the Files classes.
 */

class FileFactory {
public:
  /**
   * \brief default constructor
   */
  FileFactory();
  /**
   * \brief Update a ssh server host
   * \param sshServer the new ssh server host
   */
  void
  setSSHServer(const std::string& sshServer);
  /**
   * \brief Update a ssh port
   * \param sshPort the new ssh port
   */
  void
  setSSHPort(const unsigned int sshPort);
  /**
   * \brief Update a ssh command path
   * \param sshCommand the new ssh command path
   */
  void
  setSSHCommand(const std::string& sshCommand);
  /**
   * \brief Update a scp command path
   * \param scpCommand the new scp command path
   */
  void
  setSCPCommand(const std::string& scpCommand);

  /**
   * \brief Get the ssh file implementation
   * \param sessionServer the session server object
   * \param path the path of the file
   * \param user the file user
   * \param key the ssh private key path
   * \return The new built file
   */
  File*
  getFileServer(const SessionServer& sessionServer,
                const std::string& path,
                const std::string& user,
                const std::string& key);
private :


  /**
   * \brief The ssh host server
   */
  std::string sshServer_;
  /**
   * \brief The ssh port
   */
  unsigned int sshPort_;
  /**
   * \brief The ssh command path
   */
  std::string sshCommand_;
  /**
   * \brief The scp command path
   */
  std::string scpCommand_;
};

#endif
