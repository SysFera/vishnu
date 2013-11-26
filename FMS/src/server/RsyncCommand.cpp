/**
 * \brief Implémentation of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "RsyncCommand.hpp"
#include <string>
#include <boost/format.hpp>

RsyncCommand::RsyncCommand() : FileTransferCommand() {
  setName("Rsync");
  setLocation("/usr/bin/Rsync");
}

RsyncCommand::RsyncCommand(const bool& isRecursive,
                           const bool& useCompression,
                           int timeout) :
  FileTransferCommand("Rsync", "/usr/bin/rsync",
                      isRecursive, useCompression, "", timeout) {
  build();
}

void
RsyncCommand::build(){
  setCommand("rsync -aPq ");

  if (isRecursive()) {
    addOptions ("-r ");
  }

  if (useCompression()) {
    addOptions ("-z ");
  }

  int timeout = getTimeout();
  if (timeout) {
    addOptions(boost::str(boost::format("--timeout=%1% ") % timeout));
  }

  addOptions("--rsh=\"ssh -t -q -o StrictHostKeyChecking=no -o BatchMode=yes -o Compression=yes \" ");
}
