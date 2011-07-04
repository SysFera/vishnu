/**
 * \brief Declaration of class to build a scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef SCPCOMMAND_HPP
#define SCPCOMMAND_HPP

#include <string>
#include "TransferCommand.hpp"
/**
 * \brief A scp command class
 */
class SCPCommand{

  public:
    /**
     * \brief default constructor
     */
    SCPCommand ();
    /**
     * \brief Another constructor with parameters
     * \param isRecursive for recursive scp
     * \param useCompression to use compression or not
     */
    SCPCommand (const bool& isRecursive, const bool& useCompression=true);
    /**
     * \brief To build the command with the options
     */
    void build()

  private:


};



#endif
