/**
 * \brief Declaration of class to build a Scp command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef ScpCOMMAND_HPP
#define ScpCOMMAND_HPP

#include <string>
#include "FileTransferCommand.hpp"

/**
 * \brief The scp command
 */

class ScpCommand: public FileTransferCommand{

  public:
    /**
     * \brief The default constructor
     */
    ScpCommand ();
    /**
     * \brief A constructor by value
     * \param isRecursive a flag for recursivity
     * \param useCompression a flag to use a compression
     */

    ScpCommand (const bool& isRecursive, const bool& useCompression=true);
    /**
     * \brief To build the command
     */

    virtual  void build();

  private:


};



#endif
