/**
 * \file FMSServices.hpp
 * \brief This file lists the services provided by FMS servers.
 * \author Benjamin Depardon (benjamin.depardon@sysfera.com)
 * \date 21/02/2013
 */

#ifndef _FMSSERVICES_HPP_
#define _FMSSERVICES_HPP_


/**
 * \brief FMS services enumeration
 */
typedef enum {
  FILECOPYASYNC = 0,
  FILEMOVEASYNC,
  FILEMOVE,
  FILECOPY,
  FILEGETINFOS,
  FILECHANGEGROUP,
  FILECHANGEMODE,
  FILEHEAD,
  FILECONTENT,
  FILECREATE,
  DIRCREATE,
  FILEREMOVE,
  DIRREMOVE,
  FILETAIL,
  DIRLIST,
  REMOTEFILECOPYASYNC,
  REMOTEFILEMOVEASYNC,
  REMOTEFILECOPY,
  REMOTEFILEMOVE,
  FILETRANSFERSLIST,
  FILETRANSFERSTOP,
  UPDATECLIENTSIDETRANSFER,
  NB_SRV_FMS  // MUST always be the last
} fms_service_t;

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
  "FileTransferStop",  // 20
  "UpdateClientSideTransfer"  // 21
};

inline bool
isMachineSpecificServicesFMS(unsigned id) {
  return false;
}

#endif  // _FMSSERVICES_HPP_
