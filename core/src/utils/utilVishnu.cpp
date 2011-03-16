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
* \brief Function to get a random number
* \fn    int isMonth (const char * s)
* \return
*/
int
vishnu::isMonth (const char * s) {
  return (s[0]=='M' && s[1]=='O' && s[2]=='N' && s[3]=='T' && s[4]=='H');
}
int
vishnu::isYear (const char * s) {
  return (s[0]=='Y' && s[1]=='E' && s[2]=='A' && s[3]=='R');
}
int
vishnu::isSite (const char * s) {
  return (s[0]=='S' && s[1]=='I' && s[2]=='T' && s[3]=='E');
}
int
vishnu::isType (const char * s) {
  return (s[0]=='T' && s[1]=='Y' && s[2]=='P' && s[3]=='E');
}
int
vishnu::isUName (const char * s) {
  return (s[0]=='U' && s[1]=='N' && s[2]=='A' && s[3]=='M' && s[4]=='E');
}
int
vishnu::isMaName (const char * s) {
  return (s[0]=='M' && s[1]=='A' && s[2]=='N' && s[3]=='A' && s[4]=='M' && s[5]=='E');
}
int
vishnu::isDay (const char * s) {
  return (s[0]=='D' && s[1]=='A' && s[2]=='Y');
}
int
vishnu::isCpt (const char * s) {
  return (s[0]=='C' && s[1]=='P' && s[2]=='T');
}

/**
* \brief Parse the format and fill the array with the variable and their value
* \fn void getKeywords(int* size, Format_t* array,
*                      const char* format,
*                      int cpt, IdType type,
*                      std::string name,
*                      std::string site)
* \param size Size of the array (OUT)
* \param array Array to fill with variables and their value (OUT)
* \param format The string to parse
* \param cpt Value of the counter variable
* \param type Type of the id to generate
* \param name Name of the user or machine
* \param site Site of the machine
* \return 0 on success and -1 when an error occurs
*/
int
vishnu::getKeywords (int* size, Format_t* array, const char* format, int cpt, IdType type,
      std::string name, std::string site) {
  unsigned int i;
  *size = 0;

  char buf[500];
  memset (buf, 0, 500);

  //the current time
  ptime now = microsec_clock::local_time();
  boost::gregorian::date::ymd_type ymd = now.date().year_month_day();

  std::string monthStr = ymd.month.as_long_string();
  std::ostringstream osY, osM, osD;
  osY << ymd.year;
  std::string year = osY.str();
  osM << ymd.month;
  std::string month = osM.str();
  osD << ymd.day;
  std::string day = osD.str();

  // Loop parsing for the variables, setting their position and their value
  /* >RELAX<MISRA_6_3_1> avoid using too many brackets */
  /* >RELAX<MISRA_6_4_1> avoid using too many brackets */
  for (i=0;i<strlen (format);i++){
    if (format[i]=='$'){
      if (isDay (format+i+1)){
        array[*size].start = i;
        array[*size].end = i+3;
        array[*size].value = day;
        (*size) ++;
      }else if (isMonth (format+i+1)){
        array[*size].value = month;
        array[*size].start = i;
        array[*size].end = i+5;
        (*size) ++;
      }else if (isYear (format+i+1)){
        array[*size].start = i;
        array[*size].end = i+4;
        array[*size].value = year;
        (*size) ++;
      }else if (isCpt (format+i+1)){
        char tmp[10];
        sprintf (tmp, "%d", cpt);
        array[*size].value = std::string (tmp);
        array[*size].start = i;
        array[*size].end = i+3;
        (*size) ++;
      }else if (isSite (format+i+1)){
        array[*size].value = site;
        array[*size].start = i;
        array[*size].end = i+4;
        (*size) ++;
      }else if (isMaName (format+i+1)){
        array[*size].value = name;
        array[*size].start = i;
        array[*size].end = i+6;
        (*size) ++;
      }else if (isUName (format+i+1)){
        array[*size].value = name;
        array[*size].start = i;
        array[*size].end = i+5;
        (*size) ++;
      }else if (isType (format+i+1)) {
        switch (type){
          case 0 :
            array[*size].value = "M";
            break;
          case 1 :
            array[*size].value = "U";
            break;
          case 2 :
            array[*size].value = "J";
            break;
          case 3 :
            array[*size].value = "F";
            break;
          default :
            break;
        }
        array[*size].start = i;
        array[*size].end = i+4;
        (*size) ++;
      }
      else {
        return -1;
      }
    }
  }
  return 0;
}

/**
* \brief Function to generate an ID
* \fn std::string getGeneratedName (const char* format,
*                                   int cpt,
*                                   IdType type,
*                                   std::string name = "",
*                                   std::string site ="")
* \param format Format to use to generate the id
* \param cpt Value of the counter to use
* \param type Type of the id generated
* \param name Name of the user or machine (optionnal)
* \param site Site of the machine (optionnal)
*/
std::string
vishnu::getGeneratedName (const char* format, int cpt, IdType type,
      std::string name , std::string site ) {

  std::string res;
  res.clear ();
  res = std::string ("");
  int  i;
  int  size;
  Format_t *keywords;

  keywords = new Format_t[strlen(format)+1];
  int ret = getKeywords (&size, keywords, format, cpt, type, name, site); // Getting var and their value

  // if there is no error with the getKeywords function
  if (ret != -1) {
    // Building the id using the format and the values of the var
    if (size>0)
      res.append (format, keywords[0].start);
    else
      res = std::string (format);
    for (i=0;i<size;i++){
      res.append (keywords[i].value);
      // If other variables
      if (*(format+keywords[i].end+1) != '\0' && i!=size-1)
        res.append (format+keywords[i].end+1, keywords[i+1].start-keywords[i].end-1);
      // If text after the variable
      else if (*(format+keywords[i].end+1) != '\0' ){
        res.append (format+keywords[i].end+1, strlen (format)-keywords[i].end-1);
      }
    }
  }
  delete keywords;
  return res;
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
   * \brief Simple function to read the content of file
   * \param filePath: the path to the file
   * \return The content of the file
   */
  std::string
 vishnu::get_file_content(const std::string& filePath){

   bfs::path file (filePath);

   // Check existence of the file


   if ((false==bfs::exists(file)) || (true==bfs::is_empty(file)) ){
     throw std::runtime_error("can not read the file: " + filePath);
   }

   bfs::ifstream ifs (file);

   // Read the whole file into string

   std::stringstream ss;
    ss << ifs.rdbuf();

    return ss.str();

 }

