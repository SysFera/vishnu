/**
* \file utilVishnu.hpp
* \brief This file presents the utils functions of the vishnu system
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#ifndef _UTILVISHNU_H_
#define _UTILVISHNU_H_

#include <iostream>
#include <sstream>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace boost::posix_time;
using namespace boost::gregorian;


/**
* \enum BatchType
* \brief The type of the Batch
*/
typedef enum {
  TORQUE = 0,/*!< For TORQUE batch type */
  LOADLEVELER = 1,/*!< For LOADLEVELER batch type */
  UNDEFINED = 2 /*!< IF batch type is not defined*/
} BatchType;


/**
* \brief Function to encrypt data and password
* \param clef is the data to be encrypted
* \param salt This string is used to perturb the algorithm
* \return the string encrypted
*/
char* crypt(const char* clef, const char* salt);
/**
 * \namespace vishnu
 * \brief This naspace contains utils functions of the vishnu system
 */
namespace vishnu {

  static const std::string ROOTUSERNAME = "root";
  static const std::string UMSMAPPERNAME = "UMS";
  static const std::string TMSMAPPERNAME = "TMS";
  static const std::string IMSMAPPERNAME = "IMS";
  static const std::string FMSMAPPERNAME = "FMS";

  static const int PASSWORD_MAX_SIZE = 8;
  /**
  * \brief Generic function to convert an object to string
  * \param val is a generic data to be transformed to string
  * \return the string version of val
  */
  template <class T>
  std::string convertToString(T val) {
    std::ostringstream out;
    out << val;
    return out.str();
  }

  /**
  * \brief Function to convert a string to int
  * \param  val a value to convert to int
  * \return int value of the corresponding string
  */
  int
  convertToInt(std::string val);

  /**
   * \brief To crypt a password
   * \fn std::string cryptPassword(const std::string& salt, const std::string password)
   * \param salt The salt to use to crypt
   * \param password The password to crypt
   * \return The crypted password
   */
  std::string
  cryptPassword(const std::string& salt, const std::string password) ;

  /**
  * \brief Function to get a random number
  * \fn    int generateNumbers()
  * \return the number generated
  */
  int
  generateNumbers();

  /**
   * \brief To retrieve the password
   * \param prompt: The message inviting the user to enter his/her password
   * \return The password entered.
   */
  std::string
  takePassword(const std::string& prompt);

  /**
   * \brief Simple function to convert time
   * from string format (YYYY-MM-DD H:mm:ss) to
   * long integer format in seconds
   * \param ts: the time in string format
   * \return the time in long integer format in seconds
   */
  std::time_t
  string_to_time_t(const std::string& ts);

  /**
   * \brief Simple function to read the content of a regular file
   * \param filePath: the path to the file
   * \return The content of the file
   */
  std::string
  get_file_content(const std::string& filePath);

  /**
  * \brief Function to move file
  * \param src: the path of the file to move
  * \param dest: the destination where the file will be moved
  * \param filename: the name of the file in dest
  * \return raises an exception on error
  */
  int
  moveFile(std::string src, std::string dest,  const std::string& filename="");

  /**
   * \brief Function to check a numerical value
   * \param value The value to check
   * \return raises an exception on error
   */
  bool isNumericalValue(const std::string value);

  /**
   * \brief Function a given walltime into string
   * \param walltime The walltime to convert
   * \return the walltime converted to string
   */
   std::string convertWallTimeToString(const long& walltime);

   /**
    * \brief Function a given walltime into seconds
    * \param walltime The walltime to convert
    * \return the walltime converted to seconds
    */
   long convertStringToWallTime(const std::string& walltime);

   /**
    * \brief Function to check the job status
    * \param status the status of the job
    * \return raises an exception on error
    */
   void
   checkJobStatus(const int& status);

   /**
    * \brief Function to check the job priority
    * \param priority the priority of the job
    * \return raises an exception on error
    */
   void
   checkJobPriority(const int& priority);

   /**
    * \brief Function to check the job nbNodesAndCpuPerNode
    * \param nbNodesAndCpuPerNode the number of nodes and cpu per node
    * \return raises an exception on error
    */
   void
   checkJobNbNodesAndNbCpuPerNode(const std::string& nbNodesAndCpuPerNode);

   /**
    * \brief Function to get current time in seconds (UTC)
    * \return the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC)
    */
   time_t getCurrentTimeInUTC();

   /**
    * \brief Function to convert UTC time into localtime (seconds)
    * \param localtime the local time
    * \return the correspondant localtime (seconds)
    */
   time_t convertUTCtimeINLocaltime(const time_t& localtime);

   /**
    * \brief Function to localtime into UTC (seconds)
    * \param localtime the local time
    * \return the diffence time (seconds)
    */
   time_t convertLocaltimeINUTCtime(const time_t& localtime);

   /**
    * \brief Function to create temporary file
    * \param fileName The name of the file to create
    * \param file_content The content of the file
    */ 
   void createTmpFile(char* fileName, const std::string& file_content);

   /**
    * \brief Function to create temporary file
    * \param fileName The name of the file to create
    */
   void createTmpFile(char* fileName);

   /**
    * \brief Function to delete file
    * \param fileName The name of the file to create
    * \return 0 in case of success, another value otherwise 
    */
   int deleteFile(const char* fileName);

   /**
    * \brief Function to print success message 
    */
   inline void printSuccessMessage() {
     std::cout << "The service was performed successfully" << std::endl;
   }

}
#endif // _UTILVISHNU_H_
