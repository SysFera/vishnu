#ifndef FILEFACTORY_HH
#define FILEFACTORY_HH

#include <string>

#include "File.hh"
#include "LocalFile.hh"
#include "RemoteFile.hh"
#include "SSHFile.hh"

/* A factory class for the Files classes. */
/* Return a File pointer depending on the client/server side:
 *  - A client file is directly accessible => LocalFile or RemoteFile
 *    class.
 *  - A server file is not directly accessible => SSHFile
 */
class FileFactory {
  static std::string sshServer;
  static unsigned int sshPort;
  static std::string sshCommand;
  static std::string scpCommand;
public:
  static void setSSHServer(const std::string& sshServer);
  static void setSSHPort(const unsigned int sshPort);
  static void setSSHCommand(const std::string& sshCommand);
  static void setSCPCommand(const std::string& scpCommand);
  static File* getFileClient(const std::string& path,
                             const std::string& user="");
  static File* getFileServer(const std::string& path,
                             const std::string& user,
                             const std::string& key);
  
};

#endif
