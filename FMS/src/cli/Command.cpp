/**
 * \file Command.cpp
 * \brief This file contains a definition of a generic class to handle a command 
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */


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
