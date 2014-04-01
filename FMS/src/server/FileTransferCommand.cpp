/**
 * \brief Implementation of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "FileTransferCommand.hpp"
#include "ScpCommand.hpp"
#include "RsyncCommand.hpp"

FileTransferCommand::FileTransferCommand() :
  mname("UNKNOWN"),
  mlocation("UNKNOWN"),
  mrecursive(false),
  mcompression(false),
  mcommand("UNKNOWN"),
  mtimeout(0) {}

FileTransferCommand::FileTransferCommand(const std::string& name,
                                         const std::string& location,
                                         bool recursive,
                                         bool compression,
                                         const std::string& command,
                                         int timeout) :
  mname(name),
  mlocation(location),
  mrecursive(recursive),
  mcompression(compression),
  mcommand(command),
  mtimeout(timeout) {}

bool
FileTransferCommand::isRecursive() const {
  return mrecursive;
}

bool
FileTransferCommand::useCompression() const {
  return mcompression;
}
