/**
 * \file Command.hpp
 * \brief This file declare a generic class to handle a command 
 * \author Ibrahima Cissé (ibrahima.cisse@sysfera.com)
 */

#ifndef COMMAND_HPP
#define COMMAND_HPP
//std headers
#include <map>
//boost headers
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

/**
 * \class Command
 * \brief A generic class to handle a command
 */

class Command {
  
  public:

    /**
     * The pointer to the options type
     */

    typedef boost::shared_ptr<Options> OptionsPtr;

    /**
     * The callback type
     */ 
    typedef std::map<std::string,boost::any> CallBacks;

    /**
     * \brief The default constructor
     */

    Command();

    /**
     * \brief A constructor by value
     * \param name : The name ot the command
     */

    Command(const std::string name);


    /**
     * \brief To get  the name of the option
     */

    virtual std::string name()const;

    /**
     * \brief To display a help message about the command
     */

    virtual void usage()const;

    /**
     * \brief To parse the command line 
     */

    virtual void parse(int ac, char** av)const;

    /**
     * \brief To run the command
     */

    virtual void run(int ac, char** av)const;

    /**
     * \brief The default destructor
     */

    virtual ~Command();

  private:

    /**
     * The name of the command
     */

    std::string mname;
    /**
     * \brief The signature of the command
     */
    std::string msignature;

    /**
     * \brief The options allowed by the command
     */
    OptionsPtr mcommandOpt;
    
    /**
     * \brief The callbacks associated to the different options
     */
    CallBacks mcallBack;


};



#endif
