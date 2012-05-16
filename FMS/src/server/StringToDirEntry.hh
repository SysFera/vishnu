#ifndef STRINGTODIRENTRY_HH
#define STRINGTODIRENTRY_HH

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/foreach.hpp>
#include <boost/cstdint.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <sys/types.h>
#include "FMSVishnuException.hpp"
#include "FileTypes.hpp"

struct DateHandler {

    DateHandler (const std::string& rawDate):mrawDate(rawDate) {
    }
   std::string getDate () const {
      return mrawDate;
    }

    std::string mrawDate;
};


class  PermsHandler {

  public:
    PermsHandler (const std::string& rawPerms):mrawPerms(rawPerms){ }
    mode_t getPermissions () const {

      if (mrawPerms.size()!=9){
      throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "Can not get file permissions ");
      }
      std::istringstream iss (convertPerms(mrawPerms.substr(0,3))+convertPerms(mrawPerms.substr(3,3))+convertPerms(mrawPerms.substr(6,3)));
      mode_t res;
      iss >> std::oct >> res;
      return res;
    }

  private:
    std::string mrawPerms;
    static std::string convertPerms(const std::string& perms) {

      if (perms.size()!=3){
      throw FMSVishnuException (ERRCODE_RUNTIME_ERROR, "Can not get file permissions ");
      }
      int res=0;
      BOOST_FOREACH (char right, perms){
        res+=inOct (right);
      }
      std::stringstream ss ;
      ss << res;
      return ss.str();
    }
    static  int inOct (char right){
      int res=0;
      switch (right) {
        case 'r':
          res=4;
          break;
        case 'w':
          res=2;
          break;
        case 'x':
          res=1;
          break;
        default:
          res=0;
      }
      return res;


    }
};

/**********************************************************************/

class StringToDirEntry {
  public:

    StringToDirEntry (const std::string& rawDirEntry ):mrawDirEntry(rawDirEntry),
                                                       mperms(""),
                                                       mowner(""),
                                                       mgroup(""),
                                                       msize(0),
                                                       mdate_time(""),
                                                       mpath("") { }

    void splitter() {

      std::istringstream iss (mrawDirEntry);

      std::string date;
      std::string time;
      std::string  utcOffset;
      std::size_t nbLink;

      std::getline ((iss >> mperms >> nbLink >> mowner >> mgroup >> msize >> date >> time >> utcOffset), mpath) ;
      mdate_time=date+ " "+ time + " " + utcOffset;
    }
    mode_t getPermissions(){
      PermsHandler permsHandler(mperms.substr(1));
      return permsHandler.getPermissions();
    }
    std::string getOwner(){
      return mowner;
    }

    std::string getGroup(){
      return mgroup;
    }

     file_size_t getSize() {
      return msize;
    }
    std::string getDateTime (){

      DateHandler dateHandler (mdate_time);
      return dateHandler.getDate();
    }

    std::string getPath(){
      return mpath;
    }

  private:
    std::string mrawDirEntry;
    std::string mperms;
    std::string mowner;
    std::string mgroup;
    file_size_t msize;


    std::string mdate_time;
    std::string mpath;
};

/**********************************************************************/

#endif
