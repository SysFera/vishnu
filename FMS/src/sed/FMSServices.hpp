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
  HEARTBEATFMS,
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
  "heartbeatfmssed" // 21
};

static const bool MACHINE_SPECIC_SERVICES_FMS[NB_SRV_FMS] = {
  false,   // 0
  false,   // 1
  false,   // 2
  false,   // 3
  false,   // 4
  false,   // 5
  false,   // 6
  false,   // 7
  false,  // 8
  false,  // 9
  false,  // 10
  false,  // 11
  false,   // 12
  false,   // 13
  false,   // 14
  false,   // 15
  false,   // 16
  false,   // 17
  false,   // 18
  false,   // 19
  false,   // 20
  true // 21
};

#endif  // _FMSSERVICES_HPP_
