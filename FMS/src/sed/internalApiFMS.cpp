#include <cstdlib>
#include <sys/types.h>

#include "File.hpp"

/* Some utilitary functions. */

/* Duplicate an uid. */
uid_t* uiddup(const uid_t& uid) {
  uid_t* res = (uid_t*) calloc(sizeof(long long), 1);
  *res = uid;
  return res;
}

/* Duplicate a gid. */
gid_t* giddup(const gid_t& gid) {
  gid_t* res = (gid_t*) calloc(sizeof(long long), 1);
  *res = gid;
  return res;
}

/* Duplicate a file mode. */
mode_t* modedup(const mode_t& mode) {
  mode_t* res = (mode_t*) calloc(sizeof(long long), 1);
  *res = mode;
  return res;
}

/* Duplicate a size variable. */
file_size_t* sizedup(const file_size_t& size) {
  file_size_t* res = (file_size_t*) calloc(sizeof(long long), 1);
  *res = size;
  return res;
}

/* Duplicate a time variable. */
time_t* timedup(const time_t& time) {
  time_t* res = (time_t*) calloc(sizeof(long long), 1);
  *res = time;
  return res;
}

/* Duplicate a file type variable. */
FileType* typedup(const FileType& type) {
  FileType* res = (FileType*) calloc(sizeof(long), 1);
  *res = type;
  return res;
}
