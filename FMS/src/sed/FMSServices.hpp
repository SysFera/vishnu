/**
 * \file FMSServices.hpp
 * \brief This file lists the services provided by FMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _FMSSERVICES_HPP_
#define _FMSSERVICES_HPP_


/**
 * \brief Number of service in FMS
 */
static const unsigned int NB_SRV_FMS = 21;

static const char* SERVICES_FMS[NB_SRV_FMS] = {
  "FileCopyAsync",  // 0
  "FileMoveAsync",  // 1
  "FileMove",  // 2
  "FileCopy",  // 3
  "FileGetInfos",  // 4
  "FileChangeGroup",  // 5
  "FileChangeMode",  // 6
  "FileHead",  // 7
  "FileContent",  // 8
  "FileCreate",  // 9
  "DirCreate",  // 10
  "FileRemove",  // 11
  "DirRemove",  // 12
  "FileTail",  // 13
  "DirList",  // 14
  "RemoteFileCopyAsync",  // 15
  "RemoteFileMoveAsync",  // 16
  "RemoteFileCopy",  // 17
  "RemoteFileMove",  // 18
  "FileTransfersList",  // 19
  "FileTransferStop"  // 20
};

#endif  // _FMSSERVICES_HPP_
