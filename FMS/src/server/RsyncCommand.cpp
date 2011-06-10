/**
 * \brief Implémentation of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "RsyncCommand.hpp"
#include <string>

RsyncCommand::RsyncCommand ():FileTransferCommand() {
  setName("Rsync");
  setLocation("/usr/bin/Rsync");
}

RsyncCommand::RsyncCommand (const bool& isRecursive, const bool& useCompression):FileTransferCommand("Rsync","/usr/bin/rsync",isRecursive,useCompression) {
  build();
}

 void RsyncCommand::build(){  

  setCommand("rsync -aPq ");

  if (isRecursive()) {

    addOptions ("-r ");

  }


  if (useCompression()) {

    addOptions ("-z ");

  }

  addOptions("--rsh=\"ssh -t -q -o StrictHostKeyChecking=no -o BatchMode=yes -o Compression=yes \" ");
}
