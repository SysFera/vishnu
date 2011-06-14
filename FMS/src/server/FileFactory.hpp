
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
     * \brief Update a ssh server host
     * \param sshServer the new ssh server host
     */
    static void setSSHServer(const std::string& sshServer);
    /**
     * \brief Update a ssh port
     * \param sshPort the new ssh port
     */
    static void setSSHPort(const unsigned int sshPort);
    /**
     * \brief Update a ssh command path
     * \param sshCommand the new ssh command path
     */
    static void setSSHCommand(const std::string& sshCommand);
    /**
     * \brief Update a scp command path
     * \param scpCommand the new scp command path
     */
    static void setSCPCommand(const std::string& scpCommand);

    /**
     * \brief Get the ssh file implementation
     * \param sessionServer the session server object
     * \param path the path of the file
     * \param user the file user
     * \param key the ssh private key path
     * \return The new built file
     */
    static File* getFileServer(const SessionServer& sessionServer,
        const std::string& path,
        const std::string& user,
        const std::string& key);
  private :


    /**
     * \brief The ssh host server
     */
    static std::string sshServer;
    /**
     * \brief The ssh port
     */
    static unsigned int sshPort;
    /**
     * \brief The ssh command path
     */
    static std::string sshCommand;
    /**
     * \brief The scp command path
     */
    static  std::string scpCommand;

};

#endif
