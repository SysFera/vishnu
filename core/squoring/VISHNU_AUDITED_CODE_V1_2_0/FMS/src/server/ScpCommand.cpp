/**
 * \brief Implémentation of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "ScpCommand.hpp"
#include <string>

ScpCommand::ScpCommand ():FileTransferCommand() {
  setName("Scp");
  setLocation("/bin/Scp");
}

ScpCommand::ScpCommand (const bool& isRecursive, const bool& useCompression):FileTransferCommand("Scp","/bin/Scp",isRecursive,useCompression) {
  build();
}

 void ScpCommand::build(){  

  setCommand("scp -q ");

 
  if (isRecursive()) {

    addOptions ("-r ");

  }


  if (useCompression()) {

    addOptions ("-o Compression=yes ");

  }

  addOptions("-o StrictHostKeyChecking=no -o BatchMode=yes");
}
