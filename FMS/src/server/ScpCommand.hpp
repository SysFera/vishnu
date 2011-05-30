/**
 * \brief Declaration of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef ScpCOMMAND_HPP
#define ScpCOMMAND_HPP

#include <string>
#include "FileTransferCommand.hpp"

class ScpCommand: public FileTransferCommand{

  public:

    ScpCommand ();
    ScpCommand (const bool& isRecursive, const bool& useCompression=true);

   virtual  void build();

  private:


};



#endif
