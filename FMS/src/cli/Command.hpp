/**
 * \file Command.hpp
 * \brief This file declare a generic class to handle a command 
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP

/**
 * \class Command
 * \brief A generic class to handle a command
 */

class Command {
  
  public:

    /**
     * \brief The default constructor
     */
     
    Command();


    /**
     * \brief To display a help message about the command
     */

    virtual void usage();

    /**
     * \brief To parse the command line 
     */

    virtual void parse();

    /**
     * \brief To run the command
     */

    virtual void run();

    /**
     * \brief The default destructor
     */

    virtual ~Command();

  private:

  /**
   * \brief The signature of the command
   */
  std::string signature;

  /**
   * \brief The options allowed by the command
   */
  boost::shared_ptr<Options> commandOpt;



};




#endif
