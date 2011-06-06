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

RsyncCommand::RsyncCommand (const bool& isRecursive, const bool& useCompression):FileTransferCommand("Rsync","/usr/bin/Scp",isRecursive,useCompression) {
  build();
}

 void RsyncCommand::build(){  

  //setCommand("script -c \" rsync -aP ");
  setCommand("rsync -aP ");

  if (isRecursive()) {

    addOptions ("-r ");

  }


  if (useCompression()) {

    addOptions ("-z ");

  }

    addOptions ("--progress ");
  addOptions("--rsh=\"ssh -o StrictHostKeyChecking=no -o Compression=yes \" ");
}
