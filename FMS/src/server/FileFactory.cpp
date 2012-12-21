#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "File.hpp"
#include "SSHFile.hpp"
#include "FileFactory.hpp"

using namespace std;

// FIXME: ssh/scp commands paths are hardcoded
FileFactory::FileFactory() : sshServer_("localhost"),
                             sshPort_(22),
                             sshCommand_("/usr/bin/ssh"),
                             scpCommand_("/usr/bin/scp") {}

void
FileFactory::setSSHServer(const string& sshServer) {
  sshServer_ = sshServer;
}

void
FileFactory::setSSHPort(const unsigned int sshPort) {
  sshPort_ = sshPort;
}

void
FileFactory::setSSHCommand(const string& sshCommand) {
  sshCommand_ = sshCommand;
}

void
FileFactory::setSCPCommand(const string& scpCommand) {
  scpCommand_ = scpCommand;
}


File*
FileFactory::getFileServer(const SessionServer& sessionServer,
                           const string& path,
                           const string& user,
                           const string& key) {
  return new SSHFile(sessionServer,path, sshServer_, user, "", key, "",
                     sshPort_, sshCommand_, scpCommand_);
}
