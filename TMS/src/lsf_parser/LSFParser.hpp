/**
 * \file LSFParser.hpp
 * \brief This file presents the implementation of the LSF Parser
 * \author daouda.traore (daouda.traore@sysfera.com)
 * \date February 2012 
*/

#ifndef _LSFPARSER_H_
#define _LSFPARSER_H_
//long options flag, integers and not valid characters
static const unsigned int LONG_OPT_WE          = 0x100;
static const unsigned int LONG_OPT_RN          = 0x101;
static const unsigned int LONG_OPT_APP         = 0x102;
static const unsigned int LONG_OPT_CWD         = 0x105;
static const unsigned int LONG_OPT_UL          = 0x106;
static const unsigned int LONG_OPT_JD          = 0x107;
static const unsigned int LONG_OPT_IS          = 0x108;
static const unsigned int LONG_OPT_EO          = 0x109;
static const unsigned int LONG_OPT_OO          = 0x10a;
static const unsigned int LONG_OPT_AR          = 0x10b;
static const unsigned int LONG_OPT_WA          = 0x10c;
static const unsigned int LONG_OPT_WT          = 0x10d;
static const unsigned int LONG_OPT_ZS          = 0x10e;
static const unsigned int LONG_OPT_EP          = 0x10f;
static const unsigned int LONG_OPT_SP          = 0x110;
static const unsigned int LONG_OPT_MIG         = 0x111;
static const unsigned int LONG_OPT_SLA         = 0x112;
static const unsigned int LONG_OPT_EXT         = 0x113;
static const unsigned int LONG_OPT_LP          = 0x114;
static const unsigned int LONG_OPT_JSDL        = 0x115;
static const unsigned int LONG_OPT_JSDL_STRICT = 0x116;
static const unsigned int LONG_OPT_RNC         = 0x117;
static const unsigned int LONG_OPT_XF          = 0x118;
static const unsigned int LONG_OPT_I           = 0x119;
static const unsigned int LONG_OPT_Ip          = 0x11a;
static const unsigned int LONG_OPT_Is          = 0x11b;
static const unsigned int LONG_OPT_IS2         = 0x11c;
static const unsigned int LONG_OPT_ISp         = 0x11d;
static const unsigned int LONG_OPT_ISs         = 0x11e;
static const unsigned int LONG_OPT_IX          = 0x11f;

/**
 * \class LSFParser
 * \brief This class describes the TMS Parser
 */
class LSFParser {

public :

   /**
    * \brief Constructor
    */
   LSFParser();

  /**
   * \brief Destructor
   */
  ~LSFParser();

 /**
  * \brief Function to check a numerical value
  * \param value The value to check
  * \return true if the value is numerical otherwise false 
  */
  static bool 
  isNumerical(const std::string& value); 

 /**
  * \brief Function to date in format [hour:]minute into int
  * \param date The date to convert
  * \param compErrMsg The error message to send as exception
  * \return the converted value 
  */
  static int
  convertWallTimeToTime(const std::string& date, const std::string& compErrMsg="");

 /**
  * \brief Function to date in format [[year:][month:]day:]hour:minute into time_t
  * \param date The date to convert
  * \param compErrMsg The error message to send as exception
  * \return the converted value 
  */
  static time_t 
  convertDateToTime(const std::string& date, const std::string& compErrMsg="");

  
 /**
  * \brief Function to transform the batch directives in a script file to string arguments
  * \param pathTofile The file to parse
  * \param BATCH_PREFIX The batch directive to take into account
  * \return The vector contains the arguments
  */
  static std::vector<std::string> 
  convertScriptIntoArgv(const char* pathTofile, 
                        const std::string& BATCH_PREFIX="#BSUB"); 
 
  /**
   * \brief Function to parse a batch script
   * \param pathTofile The file to parse
   * \param req The LSF option structure to fill
   * \return zero on success, raises exception on error
   */
  static int
  parse_file(const char* pathTofile, struct submit* req);

  
  /**
   * \brief Function search vishnu generic script syntax in a file
   * \param pathTofile The file to parse
   * \param req The LSF option structure to update or to fill
   * \return raises exception on error
   */
  static void
  searchAndConvertVishnuScriptGenSyntax(const char* pathTofile, struct submit* req);

};
#endif // LSFPARSER
