#ifndef FMSUTILS_HPP
#define FMSUTILS_HPP

#include <string>
namespace vishnu {

  /**
   * @brief Build the transfer command and return the resulting command
   * @param type The type of transfer (scp, rsync...)
   * @param isRecursive Tells whether to do recusive copy or not
   * @param useCompression Tells whether to use compression or not
   * @param timeout Sets the timeout
   * @return A string
   */
  std::string
  buildTransferCommand(int type,
                       const bool& isRecursive,
                       const bool& useCompression,
                       int timeout);


  /**
   * @brief Check if one of machine is local
   * @param srcMachine The source machine
   * @param destMachine The destination machine
   * @param direction Tell the direction of the copy
   * @return
   */
  bool
  ifLocalTransferInvolved(const std::string& srcMachine,
                          const std::string& destMachine,
                          int& direction);
}
#endif // FMSUTILS_HPP
