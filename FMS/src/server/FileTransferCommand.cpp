/**
 * \brief Implementation of Helper class to build a transfer command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "FileTransferCommand.hpp"
#include "fmsUtils.hpp"
#include "FMSVishnuException.hpp"
#include <boost/format.hpp>

FileTransferCommand::FileTransferCommand(const std::string& name,
                                         const std::string& location,
                                         bool recursive,
                                         bool compression,
                                         const std::string& command,
                                         int timeout)
  : mname(name),
    mlocation(location),
    mrecursive(recursive),
    mcompression(compression),
    mtimeout(timeout),
    mcommand( vishnu::buildTransferBaseCommand(vishnu::SCP_TRANSFER,
                                           recursive,
                                           compression,
                                           timeout) )
{

}

/**
 * @brief Return a transfer manager according to a given and options
 * @param options Transfer options
 * @param compress Tells whether to use compression
 * @return
 */
FileTransferCommand*
FileTransferCommand::getTransferManager(const FMS_Data::CpFileOptions& options, bool compress)
{
  FileTransferCommand* transferManager = NULL;
  int timeout = 0;
  switch (options.getTrCommand()) {
  case vishnu::RSYNC_TRANSFER:
    transferManager = new FileTransferCommand("rsync",
                                              "/usr/bin/rsync",
                                              options.isIsRecursive(),
                                              compress,
                                              "",
                                              timeout);
    break;
  case vishnu::SCP_TRANSFER:
  default:
    transferManager = new FileTransferCommand("scp",
                                              "/usr/bin/scp",
                                              options.isIsRecursive(),
                                              compress,
                                              "",
                                              timeout);
    break;
  }
  return transferManager;
}
