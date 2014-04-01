#ifndef FMSUTILS_HPP
#define FMSUTILS_HPP

namespace vishnu {
  /**
   * \brief build ssh command
   */
  void
  buildTransferCommand(int type,
                       const bool& isRecursive,
                       const bool& useCompression,
                       int timeout);
}
#endif // FMSUTILS_HPP
