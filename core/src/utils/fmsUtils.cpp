#include "fmsUtils.hpp"
#include "constants.hpp"
#include "FMSVishnuException.hpp"
#include <boost/format.hpp>
#include <iostream>


/**
 * @brief Build the transfer command and return the resulting command
 * @param type The type of transfer (scp, rsync...)
 * @param isRecursive Tells whether to do recusive copy or not
 * @param useCompression Tells whether to use compression or not
 * @param timeout Sets the timeout
 * @return A string
 */
std::string
vishnu::buildTransferBaseCommand(int type,
                                 const bool& isRecursive,
                                 const bool& useCompression,
                                 int timeout) {

  std::string command;
  std::string options;

  if (isRecursive) {
    options.append (" -r");
  }

  switch (type) {
  case vishnu::RSYNC_TRANSFER:

    if (useCompression) {
      options.append (" -z");
    }

    if (timeout) {
      options.append(boost::str(boost::format(" --timeout=%1%") % timeout));
    }

    options.append("--rsh=\"ssh -t -q"
                   " -o UserKnownHostsFile=/dev/null"
                   " -o StrictHostKeyChecking=no"
                   " -o PasswordAuthentication=no"
                   " -o BatchMode=yes"
                   " -o Compression=yes\" ");

    command = boost::str(boost::format("rsync -aPq %1%") % options);
    break;

  case vishnu::SCP_TRANSFER:
  default:

    if (useCompression)  {
      options.append(" -o Compression=yes");
    }

    if (timeout >0) {
      options.append( boost::str(boost::format(" -o ConnectTimeout=%1%") % timeout) );
    }

    options.append(" -o UserKnownHostsFile=/dev/null"
                   " -o StrictHostKeyChecking=no"
                   " -o PasswordAuthentication=no"
                   " -o BatchMode=yes");

    command = boost::str(boost::format("scp -q %1%") % options);
    break;
  }
  return command;
}


/**
 * @brief Check if one of machine is local
 * @param srcMachine The source machine
 * @param destMachine The destination machine
 * @return
 */
bool
vishnu::ifLocalTransferInvolved(const std::string& srcMachine, const std::string& destMachine) {
  return srcMachine == "localhost"
      || destMachine == "localhost"
      || srcMachine == "127.0.0.1"
      || destMachine == "127.0.0.1";
}

