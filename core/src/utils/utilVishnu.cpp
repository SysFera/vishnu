/**
* \file utilVishnu.cpp
* \brief This file implements the utils functions of the vishnu system
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#include "utilVishnu.hpp"
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "UserException.hpp"
#include "TMS_Data.hpp"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespace

/**
* \brief Function to convert a string to int
* \param  val a value to convert to int
* \return int value of the corresponding string
*/
int
vishnu::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val);
  str >> intValue;
  return intValue;
}

/**
 * \brief To crypt a password
 * \fn std::string cryptPassword(const std::string& salt, const std::string password)
 * \param salt The salt to use to crypt
 * \param password The password to crypt
 * \return The crypted password
 */

std::string
vishnu::cryptPassword(const std::string& salt, const std::string password) {

  std::string saltTmp="$6$"+salt+"$";
  std::string encryptedPassword=crypt(password.c_str(),saltTmp.c_str());

  return encryptedPassword.substr(saltTmp.size());
}

/**
* \brief Function to get a random number
* \fn    int generate_numbers()
* \return the number generated
*/
int
vishnu::generateNumbers() {

  boost::mt19937 gen;
  gen.seed(static_cast<boost::uint32_t>(std::time(0)));
  boost::uniform_int<> dist(1, 100000);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
  return die();
}

/**
 * \brief To retrieve the password
 * \param prompt: The message inviting the user to enter his/her password
 * \return The password entered.
 */
std::string
vishnu::takePassword(const std::string& prompt){
  std::string password=getpass(prompt.c_str());
  return password;

}

/**
 * \brief Simple function to convert time
 * from string format (YYYY-MM-DD H:M:S) to
 * long integer format in seconds
 * \param ts: the time in string format
 * \return the time in long integer format in seconds
 */
std::time_t
vishnu::string_to_time_t(const std::string& ts){

  // two aliases for convenience

  namespace bps= boost::posix_time;
  namespace bg=boost::gregorian;

  bps::ptime t;

  if (std::string::npos==ts.find(":")){
    t=bps::ptime(bg::from_string(ts));

  }
  else{
    t= bps::ptime (bps::time_from_string(ts));
  }

  bps::ptime epoch(bg::date(1970,1,1));


  bps::time_duration::sec_type x = (t - epoch).total_seconds();

  return std::time_t(x);


}

/**
* \brief Simple function to read the content of the regular file
* \param filePath: the path to the file
* \return The content of the file
*/
std::string
vishnu::get_file_content(const std::string& filePath){

  bfs::path file (filePath);

  // Check the existence of file
  if (((false==bfs::exists(file)) || (true==bfs::is_empty(file)))
    || (false==bfs::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "can not read the file: " + filePath);
  }

  bfs::ifstream ifs (file);

  // Read the whole file into string

  std::stringstream ss;
  ss << ifs.rdbuf();

  return ss.str();

}

/**
* \brief Function to move file
* \param src: the path of the file to move
* \param dest: the destination where the file will be moved
* \param file: the name of the file in dest
* \return raises an exception on error
*/
int
vishnu::moveFile(std::string src, std::string dest, const std::string& filename) {

  bfs::path filePath(src);
  bfs::path fileDestPath(dest);
  bfs::path fileNewPath(src);
  if(filename.size()!=0) {
    fileNewPath = bfs::path(filename);
  }

  try {
    //If the destination does not exist, the file is created in the current directory
    if(!bfs::exists(fileDestPath)) {
      bfs::rename(filePath, bfs::path(bfs::current_path().string() / fileNewPath.filename() /*filePath.filename()*/));
    }
    else {
      bfs::rename(filePath, bfs::path(fileDestPath / fileNewPath.filename() /*filePath.filename()*/));
    }
  } catch (std::exception& e) {
      throw UserException(ERRCODE_INVALID_PARAM, e.what());
  }
  return 0;
}

/**
 * \brief Function to check a numerical value
 * \param value The value to check
 * \return raises an exception on error
 */
bool vishnu::isNumericalValue(const std::string value) {
  bool ret = (value.find_first_not_of("0123456789")==std::string::npos);
  if(!ret) {
    throw UserException(ERRCODE_INVALID_PARAM, ("Invalid numerical value: "+value));
  }
  return ret;
}


/**
 * \brief Function a given walltime into string
 * \param walltime The walltime to convert
 * \return the walltime converted to string
 */
std::string vishnu::convertWallTimeToString(const long& walltime) {

  long totalTime = walltime;
  long j = totalTime/86400;
  totalTime = walltime-j*86400;
  long h =  totalTime/3600;
  totalTime =  totalTime-h*3600;
  long m =  totalTime/60;
  long s =  totalTime-m*60;

  std::ostringstream StrWallTime;
  if(j > 0) {
    if(j < 10) {
      StrWallTime << "0" << j << ":";
    } else {
      StrWallTime <<  j << ":";
    }
  }

  if(h < 10) {
    StrWallTime << "0" << h << ":";
  } else {
    StrWallTime <<  h << ":";
  }

  if(m < 10) {
    StrWallTime << "0" << m << ":";
  } else {
    StrWallTime << m << ":";
  }

  if(s < 10) {
    StrWallTime << "0" << s;
  } else {
    StrWallTime << s ;
  }

  return StrWallTime.str();
}

/**
 * \brief Function a given walltime into seconds
 * \param walltime The walltime to convert
 * \return the walltime converted to seconds
 */
long vishnu::convertStringToWallTime(const std::string& walltime) {

  int seconds = 0;
  int minute = 0;
  int heure = 0;
  int jour = 0;
  std::string value;
  size_t found;

  size_t size = walltime.size();
  size_t pos = walltime.rfind(":");
  if(pos!=std::string::npos) {
    if((size-pos > 1)) {
      value = walltime.substr(pos+1, size-1-pos);
      if(isNumericalValue(value)) {
        seconds = convertToInt(value);
      }
    }
  } else {
    if(walltime.size() > 0) {
      value = walltime;
      if(isNumericalValue(value)) {
        seconds = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        value = walltime.substr(pos+1, size-pos-1);
        if(isNumericalValue(value)) {
          minute = convertToInt(value);
        }
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        minute = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        value = walltime.substr(pos+1, size-pos-1);
        if(isNumericalValue(value)) {
          heure = convertToInt(value);
        }
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        heure = convertToInt(value);
      }
    }
  }

  if((pos!=std::string::npos) && (pos > 0)) {
    size = pos;
    pos =  walltime.rfind(":", size-1);
    if(pos!=std::string::npos) {
      if((size-pos > 1)) {
        throw std::runtime_error("Invalid wallltime value: "+walltime);
      }
    } else {
      value = walltime.substr(0, size);
      if(isNumericalValue(value)) {
        jour = convertToInt(value);
      }
    }
  }

  long walltimeInSeconds = (jour*86400+heure*3600+minute*60+seconds);

  return walltimeInSeconds;

}

/**
* \brief Function to check the job status
* \param status the status of the job
* \return raises an exception on error
*/
void
vishnu::checkJobStatus(const int& status) {

  if ((status < -1) || (status > 7)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The status value is incorrect");
  }
}

/**
* \brief Function to check the job priority
* \param priority the priority of the job
* \return raises an exception on error
*/
void
vishnu::checkJobPriority(const int& priority) {

  if ((priority < -1) || (priority > 5)) {
    throw UserException(ERRCODE_INVALID_PARAM, "The priority value is incorrect");
  }
}
