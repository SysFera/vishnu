/**
 * \brief Declaration of class to build a scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef SCPCOMMAND_HPP
#define SCPCOMMAND_HPP

#include <string>
#include "TransferCommand.hpp"

class SCPCommand{

  public:

    SCPCommand ();
    SCPCommand (const bool& isRecursive, const bool& useCompression=true);

    void build()

  private:


};



#endif
