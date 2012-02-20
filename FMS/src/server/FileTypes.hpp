#ifndef FILETYPES_HH
#define FILETYPES_HH

#include <string>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>

#include <boost/cstdint.hpp>
#include "FMS_Data_forward.hpp"
#include "FMS_Data.hpp"
using namespace FMS_Data;


static const mode_t defaultFileAccessMode=420;
static const mode_t defaultDirectoryAccessMode=493;

/**
 * \brief File type enumeration
 */
typedef enum {
  /**
   * \brief a block 
   */
  block,
  /**
   * \brief a character 
   */
  character,
  /**
   * \brief a directory 
   */
  directory,
  /**
   * \brief a symbolic link 
   */
  symboliclink,
  /**
   * \brief a socket 
   */
  sckt,
  /**
   * \brief a fifo 
   */
  fifo,
  /**
   * \brief a regular file 
   */
  regular
} file_type_t;

/**
 * \brief  File "locality" enumeration
 */
typedef enum {
  /**
   * \brief a local file 
   */
  local,
  /**
   * \brief a remote file 
   */
  remote
} file_host_t;

/**
 * \brief  The stat structure differs depending on the system type. 
 * BSD file systems used the same structure for large and small
 * partition type.
 */
#if FS_32 | BSD_LIKE_SYSTEM
typedef struct stat file_stat_t;
typedef off_t file_size_t;
#define STAT(path, buf) stat((path), (buf))
#else
typedef struct stat64 file_stat_t;
/**
 * \brief A defintion of object size type
 */
typedef off64_t file_size_t;
/**
 * \brief The default defintion of stat function
 */
#define STAT(path, buf) stat64((path), (buf))
#endif

#endif
