/**
 * \file stop.cpp
 * This file defines the VISHNU command to stop a vishnu process
 * \author Eugène PAMBA CAPO-CHICHI(eugene.capochichi@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct ExportFunc {

  string moldSessionId;
  string mfilename;

  ExportFunc(const std::string& oldSessionId, const std::string& filename):
   moldSessionId(oldSessionId), mfilename(filename)
  {};

  int operator()(std::string sessionKey) {
    return exportCommands(sessionKey, moldSessionId, mfilename);
  }
};


boost::shared_ptr<Options>
makeExportOpt(string pgName,
            string& configFile) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  return opt;
}


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string configFile;
  string oldSessionId;
  string filename;


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeExportOpt(argv[0], configFile);

  opt->add( "oldSessionId,o",
            "represents the id of the session to export",
            HIDDEN,
            oldSessionId,1);

  opt->setPosition("oldSessionId",1);

  opt->add( "filename,f",
            "represents the file in which the data will be exported",
            HIDDEN,
            filename,1);

  opt->setPosition("filename",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv);

  //call of the api function
  ExportFunc exportFunc(oldSessionId, filename);
  return GenericCli().run(exportFunc, configFile, argc, argv);

}
