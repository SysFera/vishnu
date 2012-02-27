/**
 * \file LSFParser.hpp
 * \brief This file presents the implementation of the LSF Parser
 * \author daouda.traore (daouda.traore@sysfera.com)
 * \date April
*/

#ifndef _LSFPARSER_H_
#define _LSFPARSER_H_

static const unsigned int LONG_OPT_WE          = 0x100;
static const unsigned int LONG_OPT_RN          = 0x101;
static const unsigned int LONG_OPT_APP         = 0x102;
static const unsigned int LONG_OPT_CWD         = 0x103;
static const unsigned int LONG_OPT_UL          = 0x104;
static const unsigned int LONG_OPT_JD          = 0x105;
static const unsigned int LONG_OPT_IS          = 0x106;
static const unsigned int LONG_OPT_EO          = 0x107;
static const unsigned int LONG_OPT_OO          = 0x108;
static const unsigned int LONG_OPT_AR          = 0x109;
static const unsigned int LONG_OPT_WA          = 0x110;
static const unsigned int LONG_OPT_WT          = 0x111;
static const unsigned int LONG_OPT_ZS          = 0x112;
static const unsigned int LONG_OPT_EP          = 0x113;
static const unsigned int LONG_OPT_SP          = 0x114;
static const unsigned int LONG_OPT_MIG         = 0x115;
static const unsigned int LONG_OPT_SLA         = 0x116;
static const unsigned int LONG_OPT_EXT         = 0x117;
static const unsigned int LONG_OPT_LP          = 0x118;
static const unsigned int LONG_OPT_JSDL        = 0x119;
static const unsigned int LONG_OPT_JSDL_STRICT = 0x120;
static const unsigned int LONG_OPT_RNC         = 0x121;
static const unsigned int LONG_OPT_XF          = 0x122;


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

  static bool 
  isNumerical(const std::string& value); 

   /**
   * \brief To parse a given file
   * \return raises an exception
   */
  static int
  parse_file(const char* pathTofile, struct submit* req);

private :

  /////////////////////////////////
  // Attributes
  /////////////////////////////////

};
#endif // LSFPARSER
