#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>

#include "DIET_client.h"
#include "DIET_Dagda.h"

#include "SessionServer.hpp"

#include "LocalFile.hh"
#include "RemoteFile.hh"
#include "File.hh"

using namespace std;

/* Default constructor. */
LocalFile::LocalFile() : File() {
  setHost("localhost");
  upToDate = false;
}

/* Standard constructor.
 * Use the file path as argument. */
LocalFile::LocalFile(const SessionServer& sessionServer,const string& path) : File(sessionServer,path) {
  setHost("localhost");
  upToDate = false;
}

/* Copy constructor. */
LocalFile::LocalFile(const LocalFile& file) : File(file) {
  upToDate = file.isUpToDate();
}

/* Copy operator. */
LocalFile& LocalFile::operator=(const LocalFile& file) {
  File::operator=(file);
  upToDate = file.isUpToDate();
  return *this;
}

/* Return true if the file informations are up to date. */
bool LocalFile::isUpToDate() const {
  return upToDate;
}

/* Obtain the information of a local file. Uses the POSIX standard functions. */
void LocalFile::getInfos() const {
  file_stat_t fileStat;
  struct group* grp;
  struct passwd* usr;
  
  if (STAT(getPath().c_str(), &fileStat)==-1) {
    exists(false);
    return;
  }
  setPerms(fileStat.st_mode & ACCESSPERMS);
  setUid(fileStat.st_uid);
  setGid(fileStat.st_gid);
  setSize(fileStat.st_size);
  setAtime(fileStat.st_atime);
  setMtime(fileStat.st_mtime);
  setCtime(fileStat.st_ctime);
  if (S_ISBLK(fileStat.st_mode))
    setType(block);
  if (S_ISCHR(fileStat.st_mode))
    setType(character);
  if (S_ISDIR(fileStat.st_mode))
    setType(directory);
  if (S_ISLNK(fileStat.st_mode))
    setType(symboliclink);
  if (S_ISSOCK(fileStat.st_mode))
    setType(sckt);
  if (S_ISFIFO(fileStat.st_mode))
    setType(fifo);
  if (S_ISREG(fileStat.st_mode))
    setType(regular);
  
  usr = getpwuid(fileStat.st_uid);
  grp = getgrgid(fileStat.st_gid);
  
  if (usr!=NULL)
    setOwner(usr->pw_name);
  if (grp!=NULL)
    setGroup(grp->gr_name);
  
  exists(true);
  upToDate=true;
}

/* Change the group of a local file. */
int LocalFile::chgrp(const string& group) {
  struct group* grp;
  int result;

  if (!exists()) throw runtime_error(getPath()+" does not exist");
  
  grp = getgrnam(group.c_str());
  if (grp==NULL) throw runtime_error("Unknown group "+group);
  
  result = ::chown(getPath().c_str(), (uid_t) -1, grp->gr_gid);
  if (result==-1)
    switch (errno) {
      case EACCES:
        throw runtime_error("Access denied to "+getPath());
      case EFAULT:
        /* Should never happen because we use the path accessor.
           This error can traduce previous memory corruption. */
        throw runtime_error("Internal bug - function: "+string(__FUNCTION__)+" ("+string(__FILE__)+")"+
                            " please send a mail to contact@sysfera.com"+
                            " indicating this error message");
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOTDIR:
        throw runtime_error("A component of the path prefix is not a directory.");
      default:
        throw runtime_error("Unknown error while accessing "+getPath());
    }
  setGroup(group);
  return result;
}

/* Change the local file permissions. */
int LocalFile::chmod(const mode_t mode) {
  int result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  result = ::chmod(getPath().c_str(), mode);
  if (result==-1)
    switch (errno) {
      case EACCES:
        throw runtime_error("Access denied to "+getPath());
      case EFAULT:
        /* Should never happen because we use the path accessor.
         This error can traduce previous memory corruption. */
        throw runtime_error("Internal bug - function: "+string(__FUNCTION__)+" ("+string(__FILE__)+")"+
                            " please send a mail to contact@sysfera.com"+
                            " indicating this error message");
      case EINTR:
        throw runtime_error("A signal interrupted the chmod operation");
      case EIO:
        throw runtime_error("IO error accessing to "+getPath());
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOTDIR:
        throw runtime_error("A component of the path prefix is not a directory.");
      case EPERM:
        throw runtime_error("Permission error while changing file "+getPath()+" mode");
      default:
        throw runtime_error("Unknown error while accessing "+getPath());
    }
  setPerms(mode);
  return result;
}



/* Print the head of the local file.*/
string LocalFile::head(const unsigned int nline) {
  char buffer[10240];
  ifstream input(getPath().c_str());
  unsigned int count;
  string result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (!input.is_open()) throw runtime_error("Cannot open "+getPath());

  for (count=0; count < nline && input.good(); ++count) {
    input.getline(buffer, 10240);
    result+=buffer;
    result+="\n";
  }
  input.close();
  return result;
}
/* Print the content of file*/
std::string LocalFile::getContent(){

  return string(); 
}


/* Create the file represented by this File object. */
int LocalFile::mkfile(const mode_t mode) {
  int result=0;

  return result;
}

/* Create the directory represented by this File object. */
int LocalFile::mkdir(const mode_t mode) {
  int result;
  if (exists())
    throw runtime_error(getPath()+ " already exists");
  
  result = ::mkdir(getPath().c_str(), mode);
  if (result==-1) {
    switch (errno) {
      case EACCES:
        throw runtime_error("Access denied to "+getPath());
      case EDQUOT:
        throw runtime_error("Quota error");
      case EEXIST:
        throw runtime_error(getPath()+ " already exists");
      case EFAULT:
        /* Should never happen because we use the path accessor.
         This error can traduce previous memory corruption. */
        throw runtime_error("Internal bug - function: "+string(__FUNCTION__)+" ("+string(__FILE__)+")"+
                            " please send a mail to contact@sysfera.com"+
                            " indicating this error message");
      case EIO:
        throw runtime_error("IO error accessing to "+getPath());
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case EMLINK:
        throw runtime_error("Parent directory reaches the maximum number of links");
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOSPC:
        throw runtime_error("Not enough space on the device to create "+getPath());
      case ENOTDIR:
        throw runtime_error("A component of the path prefix is not a directory.");
      case EPERM:
        throw runtime_error("Permission error while changing file "+getPath()+" mode");
      case EROFS:
        throw runtime_error("Parent directory resides on a read-only file system");
      default:
        throw runtime_error("Unknown error while creating "+getPath());
    }
  }
  exists(true);
  return result;
}

/* Remove this local file. Uses the C-standard function "unlink". */
int LocalFile::rm() {
  int result;
  if (!exists()) throw runtime_error(getPath()+" does not exist");

  result = ::unlink(getPath().c_str());
  if (result==-1)
    switch (errno) {
      case EACCES:
        throw runtime_error("Access denied to "+getPath());
      case EBUSY:
        throw runtime_error(getPath()+" is currently used by another process");
      case EFAULT:
        /* Should never happen because we use the path accessor.
         This error can traduce previous memory corruption. */
        throw runtime_error("Internal bug - function: "+string(__FUNCTION__)+" ("+string(__FILE__)+")"+
                            " please send a mail to contact@sysfera.com"+
                            " indicating this error message");
      case EIO:
        throw runtime_error("IO error accessing to "+getPath());
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOTDIR:
        throw runtime_error("A component of the path of "+getPath()+
                            " prefix is not a directory.");
      case EPERM:
        throw runtime_error("Permission error");
      case EROFS:
        throw runtime_error(getPath()+" is located on a read-only file system");
      default:
        throw runtime_error("Error while removing "+getPath());
    }
  exists(false);
  upToDate=false;
  return result;
}

/* Remove this local directory. Uses the C-standard rmdir function. */
int LocalFile::rmdir() {
  int result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  result = ::rmdir(getPath().c_str());
  if (result==-1)
    switch (errno) {
      case EACCES:
        throw runtime_error("Access denied to "+getPath());
      case EBUSY:
        throw runtime_error(getPath()+" is currently used by another process");
      case EFAULT:
        /* Should never happen because we use the path accessor.
         This error can traduce previous memory corruption. */
        throw runtime_error("Internal bug - function: "+string(__FUNCTION__)+" ("+string(__FILE__)+")"+
                            " please send a mail to contact@sysfera.com"+
                            " indicating this error message");
      case EIO:
        throw runtime_error("IO error accessing to "+getPath());
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOTDIR:
        throw runtime_error("A component of the path of "+getPath()+
                            " prefix is not a directory.");
      case ENOTEMPTY:
        throw runtime_error(getPath()+" is not empty");
      case EPERM:
        throw runtime_error("Permission error");
      case EROFS:
        throw runtime_error(getPath()+" is located on a read-only file system");
      default:
        throw runtime_error("Error while removing "+getPath());
    }
  exists(false);
  upToDate=false;
  return result;
}

/* */
string LocalFile::tail(const unsigned int nline) {
  string result;
  if (!exists()) throw runtime_error(getPath()+" does not exist");

  // Perform tail
  
  
  return result;
}

/* List the files of this local directory. */
list<string> LocalFile::ls(const LsDirOptions& options) const {
  list<string> result;
  DIR* dir;
  struct dirent* entry;

  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (getType()!=directory)
    throw runtime_error(getPath()+" is not a directory");

  dir = opendir(getPath().c_str());
  if (!dir)
    switch (errno) {
      case EACCES:
        throw runtime_error("Permission denied to access "+getPath());
      case ELOOP:
        throw runtime_error("Too many symbolic links in the path. There is probably"
                            "a looping symbolic link in "+getPath());
      case ENAMETOOLONG:
        throw runtime_error("File path too long: "+getPath());
      case ENOENT:
        throw runtime_error("A component in "+getPath()+" does not exist.");
      case ENOTDIR:
        throw runtime_error("A component of the path of "+getPath()+
                            " is not a directory.");
      default:
        throw runtime_error("Error opening "+getPath());
    }

  while ((entry=readdir(dir))) {
    result.push_back(entry->d_name);
  }
  return result;
}
