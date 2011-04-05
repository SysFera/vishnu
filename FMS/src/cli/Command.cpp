/**
 * \file Command.cpp
 * \brief This file contains a definition of a generic class to handle a command 
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */

#include "Command.hpp"

/**
 * \brief The default constructor
 */

Command():mname("unkown"),msignature("unknown"){}

Command (const std::string name):mname(name){}


/**
 * \brief To get  the namr of the option
 */

virtual std::string name()const{return mname;};


/**
 * \brief To display a help message about the command
 */

virtual void usage();

/**
 * \brief To parse the command line 
 */

virtual void parse(int ac, char** av);

/**
 * \brief To run the command
 */

virtual void run(int ac, char** av);

/**
 * \brief The default destructor
 */

virtual ~Command();

/**
 * \brief The signature of the command
 */
std::string signature;



  /**
   * \brief The options allowed by the command
   */
  boost::shared_ptr<Options> commandOpt;

/**
 * \brief The callbacks 
 */
  CallBacks callback; 

};




#endif
