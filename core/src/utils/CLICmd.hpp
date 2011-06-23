/**
 * \file CLICmd.hpp
 * \brief This file defines the CLICmd class, that parses the CLI and fill options
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __CLICMD__
#define __CLICMD__

#include "Options.hpp"
#include "cliError.hpp"

using namespace std;

/**
 * \brief This class represents the class to parse the cli
 * \class CLICmd
 */
class CLICmd{
private :
  /**
   * \brief The argc value
   */
  int    		     margc;
  /**
   * \brief The argv value
   */
  char** 		     margv;
  /**
   * \brief The option of the call
   */
  boost::shared_ptr<Options> mop;
protected:
public:
  /**
   * \brief To get all the parameters for the C++ API call
   * \fn int parse(const func1 & s)
   * \param s: The environment
   * \return An error code depending on the success of the call
   */
  int parse(const func1 & s);
  /**
   * \brief Constructor
   * \fn CLICmd(int argc, char** argv, boost::shared_ptr<Options> op, string dietConfig)
   * \param argc: argc from C program
   * \param argv: argv from C program
   * \param op: Option to fill (OUT)
   */
  CLICmd(int argc, char** argv, boost::shared_ptr<Options> op);
  /**
   * \brief Default destructor
   * \fn ~CLICmd()
   */
  ~CLICmd();
};
#endif
