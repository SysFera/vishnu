/**
 * \brief Implémentation of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "ScpCommand.hpp"
#include <string>
#include <boost/format.hpp>

ScpCommand::ScpCommand() : FileTransferCommand() {
  setName("Scp");
  setLocation("/bin/Scp");
}

ScpCommand::ScpCommand(const bool& isRecursive,
                       const bool& useCompression,
                       int timeout) :
  FileTransferCommand("Scp", "/bin/Scp",
                      isRecursive, useCompression, "", timeout) {
  build();
}

void
ScpCommand::build() {
  setCommand("scp -q ");

  if (isRecursive()) {
    addOptions ("-r ");
  }

  if (useCompression()) {
    addOptions ("-o Compression=yes ");
  }

  int timeout = getTimeout();
  if (timeout) {
    addOptions(boost::str(boost::format("-o ConnectTimeout=%1% ") % timeout));
  }

  addOptions("-o StrictHostKeyChecking=no -o BatchMode=yes");
}
