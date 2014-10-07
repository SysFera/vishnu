#include "cliUtil.hpp"
#include <boost/format.hpp>
#include <stdexcept>                    // for runtime_error
#include "Options.hpp"                  // for Options, Configuration, etc
#include "vishnu_version.hpp"           // for VISHNU_VERSION

/**
 * \brief helper function to display help about a specific command
 * \param opt: describes all options allowed by the command
 * \param signature: an optional message that is added to the help
 *                   message bellow the signature of the command
 */
void
helpUsage(const Options& opt, const std::string& signature)
{
  std::cout << boost::format("\nVersion: %1%\n\n"
                        "Usage: %2% [options] %3%\n"
                        "%4%\n"
                        "%5%\n")
    % VISHNU_VERSION
    % opt.getConfiguration()->getPgName()
    % opt.getPositionalOptString()
    % signature
    % opt;
}


/**
 * \brief helper function to display error about a specific command
 * \param cli   :The name of the command
 * \param errMsg: The error message to display
 * \param err   : The error type
 */
void
errorUsage(const std::string & cli,const std::string& errMsg,const ErrorType& err){

  std::cerr << boost::format("%1%: %2%\n") % cli % errMsg;

  if (err == PARAMERROR) {
    std::cerr << boost::format("To get help, try << %1% -h >>\n") % cli;
  }
}


/**
 * \brief Helper function to display information (error or usage) about a specific command
 * \param opt   :describes all options allowed by the command
 * \param mess: The help usage message to display
 * \param eWhat   : The error message to display
 * \return 0 if an help is required or 1 if an error must me displayed
 */
int
usage(const Options & opt, const std::string& mess, const std::string& eWhat)
{
  int res(0);
  if (opt.count("help")) {
    helpUsage(opt, mess);
  } else {
    errorUsage(opt.getConfiguration()->getPgName(), eWhat);
    res = 1;
  }

  return res;
}

/**
 * \brief A helper function which check if VISHNU_CONFIG_FILE is set
 *\param opt: describes all options allowed by the command
 *\exception raise a runtime exception if the VISHNU_CONFIG_FILE is not set
 */
void
checkVishnuConfig(const Options & opt)
{
  if ((opt.count("configFile") == 0) && (opt.count("help") == 0)) {
    throw std::runtime_error("Set the VISHNU_CONFIG_FILE in your environment variable");
  }

}
