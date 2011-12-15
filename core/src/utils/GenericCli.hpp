#ifndef GENERIC_CLI_HPP
#define GENERIC_CLI_HPP


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
//#include "cliError.hpp"
#include "UserException.hpp"
#include "cliUtil.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief A generic class to handle some command line operations like run a
 * function
 * \param function the api function
 * \param dietConfig the VISHNU client config file
 * \param ac The number of command line arguments
 * \param av The command line arguments
 * \return 0 if it succeeds or an error code otherwise
 */


class GenericCli {

  public:

    /**
     * \brief A generic class to handle some command line operations like run a
     * function
     * \param function the api function
     * \param dietConfig the VISHNU client config file
     * \param ac The number of command line arguments
     * \param av The command line arguments
     * \return 0 if it succeeds or an error code otherwise
     */


    template <class ApiFunc>
      int run(ApiFunc function, std::string dietConfig, int ac, char*  av[]) {

        try{

          // initializing DIET

          if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

            errorUsage(av[0],dietErrorMsg,EXECERROR);

            return  ERRCODE_CLI_ERROR_DIET ;
          }

          // get the sessionKey
          std::string sessionKey=getLastSessionKey(getppid());

          // DIET call 
          if(false==sessionKey.empty()){
            printSessionKeyMessage();
            
            //call of the api fuction
            int ret =function(sessionKey);

            if (VISHNU_OK == ret){
            printSuccessMessage();
            } 
            return ret;

          }
          errorUsage(av[0],"cannot retrieve sessionKey");

        } catch(VishnuException& e){// catch all Vishnu runtime error
          std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
          errorUsage(av[0], msg,EXECERROR);
          //check the bad session key
          if (checkBadSessionKeyError(e)){
            removeBadSessionKeyFromFile(getppid());
          }
          return e.getMsgI() ;
        } catch(std::exception& e){// catch all std runtime error
          errorUsage(av[0],e.what());
          return ERRCODE_CLI_ERROR_RUNTIME;
        }

      }

    /**
     * \brief A generic class to handle some command line operations like run a
     * functioni without managing the session
     * \param function the api function
     * \param dietConfig the VISHNU client config file
     * \param ac The number of command line arguments
     * \param av The command line arguments
     * \return 0 if it succeeds or an error code otherwise
     */


    template <class ApiFunc>
      int runWithoutSessionKey(ApiFunc function, std::string dietConfig, int ac, char*  av[]) {

        try{

          // initializing DIET

          if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

            errorUsage(av[0],dietErrorMsg,EXECERROR);

            return  ERRCODE_CLI_ERROR_DIET ;
          }

          //call of the api fuction
          int ret =function();

          if (VISHNU_OK == ret) {
            printSuccessMessage(); 
        }
        return ret;

        } catch(VishnuException& e){// catch all Vishnu runtime error
          std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
          errorUsage(av[0], msg,EXECERROR);
          return e.getMsgI() ;
        } catch(std::exception& e){// catch all std runtime error
          errorUsage(av[0],e.what());
          return ERRCODE_CLI_ERROR_RUNTIME;
        }

      }


    /**
     * \brief To parse command line options 
     * \param opt the description of the command line options
     * \param isEmpty To check if command line does not contain option
     * \param ac The number of command line arguments
     * \param av The command line arguments 
     * \param signature The signature of the command
     */
    void processListOpt(const boost::shared_ptr<Options>& opt, bool& isEmpty, int ac, char*  av[], 
        const std::string& signature=std::string()) {

      CLICmd cmd = CLICmd (ac, av, opt);
      try {
        opt->parse_cli(ac,av);

        isEmpty=opt->empty();//if no value was given in the command line
        // Parse the cli and setting the options found
        int ret = cmd.parse(env_name_mapper());

        if (ret != VISHNU_OK){
          helpUsage(*opt,"[option] "+signature);
          exit(ret);
        }

        // PreProcess (adapt some parameters if necessary)
        checkVishnuConfig(*opt);
        if ( opt->count("help")){
          helpUsage(*opt,"[option] "+signature);
          exit(VISHNU_OK);
        }

      }
      catch(po::error& e){ // catch all other bad parameter errors
        helpUsage(*opt,"[option] "+signature);
        exit(ERRCODE_INVALID_PARAM);
      }
      catch(std::exception& e){// catch all std runtime error
        errorUsage(av[0],e.what());
        exit(ERRCODE_INVALID_PARAM);
      }

    }
};

#endif 
