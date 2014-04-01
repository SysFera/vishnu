#include "fmsUtils.hpp"
#include "constants.hpp"
#include "FMSVishnuException.hpp"
#include <boost/format.hpp>
#include <iostream>

void
vishnu::buildTransferCommand(int type,
                             const bool& isRecursive,
                             const bool& useCompression,
                             int timeout) {

  std::string command;
  std::string options;

  if (isRecursive) {
    options.append (" -r");
  }

  switch (type) {
  case vishnu::SCP_TRANSFER:

    if (useCompression)  {
      options.append(" -o Compression=yes");
    }

    if (timeout >0) {
      options.append( boost::str(boost::format(" -o ConnectTimeout=%1%") % timeout) );
    }

    options.append(" -o StrictHostKeyChecking=no -o BatchMode=yes");

    command = boost::str(boost::format("scp -q %1%") % options);
    break;

  case vishnu::RSYNC_TRANSFER:

    if (useCompression) {
      options.append (" -z");
    }

    if (timeout) {
      options.append(boost::str(boost::format(" --timeout=%1%") % timeout));
    }

    options.append("--rsh=\"ssh -t -q -o StrictHostKeyChecking=no -o BatchMode=yes -o Compression=yes \" ");

    command = boost::str(boost::format("rsync -aPq %1%") % options);
    break;

  default:
    throw FMSVishnuException(ERRCODE_INVALID_PARAM,
                             boost::str(boost::format("unknown transfer type: %1%") % type));
    break;
  }
}

