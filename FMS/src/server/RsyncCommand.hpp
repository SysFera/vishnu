/**
 * \brief Declaration of class to build a Rsync command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef RsyncCOMMAND_HPP
#define RsyncCOMMAND_HPP

#include <string>
#include "FileTransferCommand.hpp"

/**
 * \brief The rsync command
 */
class RsyncCommand: public FileTransferCommand{

  public:

    /**
     * \brief The default constructor
     */
    RsyncCommand ();
    
    /**
     * \brief A constructor by value
     * \param isRecursive a flag for recursivity
     * \param useCompression a flag to use a compression
     */
    RsyncCommand (const bool& isRecursive, const bool& useCompression=true);

    /**
     * \brief To build the command
     */
   virtual  void build();

  private:


};



#endif
