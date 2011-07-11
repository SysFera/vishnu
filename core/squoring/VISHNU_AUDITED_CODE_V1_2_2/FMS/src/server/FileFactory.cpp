#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "File.hpp"
#include "SSHFile.hpp"
#include "FileFactory.hpp"

using namespace std;



void FileFactory::setSSHServer(const string& sshServer) {
  FileFactory::sshServer = sshServer;
}

void FileFactory::setSSHPort(const unsigned int sshPort) {
  FileFactory::sshPort = sshPort;
}

void FileFactory::setSSHCommand(const string& sshCommand) {
  FileFactory::sshCommand = sshCommand;
}

void FileFactory::setSCPCommand(const string& scpCommand) {
  FileFactory::scpCommand = scpCommand;
}


File* FileFactory::getFileServer(const SessionServer& sessionServer,
                                 const string& path,
                                 const string& user,
                                 const string& key) {
  
    return new SSHFile(sessionServer,path, sshServer, user, "", key, "",
                       sshPort, sshCommand, scpCommand);
}

string FileFactory::sshServer = "localhost";
unsigned int FileFactory::sshPort = 22;
string FileFactory::sshCommand = "/usr/bin/ssh";
string FileFactory::scpCommand = "/usr/bin/scp";
