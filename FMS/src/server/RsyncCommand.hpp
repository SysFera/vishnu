/**
 * \brief Declaration of class to build a Rsync command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef RsyncCOMMAND_HPP
#define RsyncCOMMAND_HPP

#include <string>
#include "FileTransferCommand.hpp"

class RsyncCommand: public FileTransferCommand{

  public:

    RsyncCommand ();
    RsyncCommand (const bool& isRecursive, const bool& useCompression=true);

   virtual  void build();

  private:


};



#endif
