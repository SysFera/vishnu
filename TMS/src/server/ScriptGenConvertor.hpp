/**
 * \file ScriptGenConvertor.hpp
 * \brief This file contains the VISHNU ScriptGenConvertor class.
 * \authors Ibrahima Cisse (ibrahima.cisse@sysfera.com) and 
 * Daouda Traore (daouda.traore@sysfera.com)
 * \date May 2011
 */

#ifndef _ScriptGenConvertor_H_
#define _ScriptGenConvertor_H_

#include <iostream>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#include <iterator>
#include <unistd.h>
#include <stdexcept>
#include <sys/wait.h>
#include <boost/shared_ptr.hpp>

#include "utilVishnu.hpp"

using namespace std;

//The generic script syntaxe
static std::string prefix = "vishnu";
static const std::string group               = prefix+"_group";
static const std::string jobName             = prefix+"_job_name";
static const std::string jobOutput           = prefix+"_output";
static const std::string jobError            = prefix+"_error";
static const std::string jobWallClockLimit   = prefix+"_wallclocklimit";
static const std::string workingDir          = prefix+"_workingdir";
static const std::string notify_user         = prefix+"_notify_user";
static const std::string nodes               = prefix+"_job_nodes";
static const std::string queue               = prefix+"_queue";
static const std::string priority            = prefix+"_userpriority";
static const std::string mem                 = prefix+"_memory";
static const std::string cpuTime             = prefix+"_cputime";
static const std::string loadLevelerSec      = "loadleveler_sec";
static const std::string torqueSec           = "torque_sec";
static const std::string commandSec          = "command_sec";

/**
 * \class ScriptGenConvertor
 * \brief ScriptGenConvertor class implementation
 */
class ScriptGenConvertor {

  public :

    /**
     * \brief Constructor 
     * \param batchType the type of the batch scheduler
     * \param scriptGenContent the generic script to convert
     */
    ScriptGenConvertor(const int batchType, 
                       const std::string&  scriptGenContent);

    /**
     * \brief Function to parse the generic script 
     * \param errorMessage is the message of the errors occured during the parsing
     * \return 0 if success, -1 if if failure 
     */ 
    int 
    parseFile(std::string& errorMessage);

    /**
     * \brief Function to return the converted script 
     */ 
    std::string 
    getConvertedScript();

    /**
     * \brief Function to initialize the generic script syntax 
     */
    void 
    initializeTableOfSymbols(); 

    /**
     * \brief Function to check if the script is generic
     */
    bool 
    scriptIsGeneric();

    /**
     * \brief Destructor
     */
    ~ScriptGenConvertor();

  private:

    /**
     * \brief The type of the batch scheduler 
     */
    const int mbatchType;
    
    /**
     * \brief The generic script 
     */
    const std::string mscriptGenContent;

    /**
     * \brief The vector containing the batch scheduler directive syntaxes 
     *  converted and theirs value
     */
    std::vector< pair<std::string, std::string> > mjobDescriptor;
  
    /**
     * \brief The map containing the generic script directive key word and 
     * its correspond value for each batch scheduler  
     */ 
    std::map<std::string, std::string>  mconversionTable;
  
    /**
     * \brief The table contanining the generic script syntaxe
     */ 
    std::vector<std::string> mtableOfSymbols;
  
    /**
     * \brief The end key word of the converted script 
     */ 
    std::string mendScript;
};

/**
 * \brief Function to return the generic script convertor 
 * \param batchType The type of the batch scheduler
 * \param scriptGenContent The content of the script to convert
 * \return The generic script convertor
 */
boost::shared_ptr<ScriptGenConvertor>  
vishnuScriptGenConvertor(const int batchType, 
                         const std::string& scriptGenContent);

#endif
