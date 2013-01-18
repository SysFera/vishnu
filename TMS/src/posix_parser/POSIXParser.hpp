/**
 * \file POSIXParser.hpp
 * \brief This file contains the VISHNU posix parser class.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#ifndef __POSIXPARSER__HH__
#define __POSIXPARSER__HH__


#include <string>
#include <time.h>

/**
 * \class Definition
 * \brief The definitions key = value on a line to parse
 */
class Definition {
  public:
/**
 * \brief The key in the line, vishnu_* usually
 */
    std::string key;
/**
 * \brief The value of the variable
 */
    std::string value;
};


/**
 * \class JobCtx
 * \brief The context for a job
 */
class JobCtx {
  public:
/**
 *\brief The working directory
 */
   std::string vishnu_working_dir;
/**
 *\brief The name of the job
 */
   std::string vishnu_job_name;
/**
 *\brief The output file
 */
   std::string vishnu_output;
/**
 *\brief The error file
 */
   std::string vishnu_error;
/**
 *\brief The clock limit
 */
   ::time_t vishnu_wallclocklimit;
/**
 *\brief To add a definition "current"
 */
   void
   AddDefinition(Definition current);
} ;


/**
 * \brief Return true on good parsing
 * \param command the command file to parse
 * \param context the context (INOUT) of the job
 * \return true on good parsing
 */
bool
ParseCommand(char* command,JobCtx& context);

/**
 * \brief Test function
 */
bool
ParseString();

#endif
