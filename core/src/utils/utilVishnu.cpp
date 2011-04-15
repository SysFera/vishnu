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
* \brief Function to copy file from Dagda
* \param src: the path of the file to copy
* \param dest: the destination to copy file
* \return raises an exception on error
*/
int
vishnu::copyDagdaFile(std::string src, std::string dest) {

  bfs::path filePath(src);
  bfs::path fileDestPath(dest);

  try {
    //If the destination does not exist, the file is created locally
    if(!bfs::exists(fileDestPath)) {
      bfs::rename(filePath, bfs::path(bfs::current_path().string() / filePath.filename()));
    }
    else {
      bfs::rename(filePath, bfs::path(fileDestPath / filePath.filename()));
    }
  }
  catch(std::exception& e) {
    throw UserException(ERRCODE_INVALID_PARAM, e.what());
  }

  return 0;
}
