/**
 * \brief Implémentation of class to build a scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "SCPCommand.hpp"
#include <string>

SCPCommand::SCPCommand ():mname("SCP"),mlocation("/bin/scp") {
}

SCPCommand::SCPCommand (const bool& isRecursive, const bool& useCompression):SCPCommand(),mrecursive(isRecursive),museCompression(useCompression) {
  build();
}

virtual void SCPCommand::build(){  

  setCommand("script \" ");


  addOptions("-o StrictHostKeyChecking=no ");

  if (isRecursive()) {

    addOptions ("-r ");

  }


  if (useCompression()) {

    addOptions ("o Compression=yes ");

  }

}
