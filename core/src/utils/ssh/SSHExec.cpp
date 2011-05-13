#include <string>
#include <cstring>
#include <stdexcept>

#include "SSHExec.hpp"

using namespace std;

SSHExec::SSHExec(const string& sshCommand, const string& scpCommand,
                 const string& server, unsigned int sshPort,
                 const string& userName,
                 const string& password, const string& publicKey,
                 const string& privateKey,
		 boost::function1<SSHResult*, string>& func) :
  msshCommand(sshCommand), mscpCommand(scpCommand), mserver(server),
  msshPort(sshPort), muserName(userName), mpassword(password), mpublicKey(publicKey),
mprivateKey(privateKey), mfunc(func)
{

}


SSHResult*
SSHExec::exec(const string& cmd) const {
  return NULL;  
}

string
SSHExec::getSSHcmd() {
  return msshCommand;
}

string
SSHExec::getPrivateKey() {
  return mprivateKey;
}

string
SSHExec::getUsername() {
  return muserName;
}

int
SSHExec::getSSHPort() {
  return msshPort;
}

string
SSHExec::getServer() {
  return mserver;
}
