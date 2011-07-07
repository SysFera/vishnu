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
#include "api_ims.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct ExportFunc {

  string moldSessionId;
  string mfilename;
  IMS_Data::ExportOp mexportOp;

  ExportFunc(const std::string& oldSessionId, const std::string& filename,
             const IMS_Data::ExportOp& exportOp):
   moldSessionId(oldSessionId), mfilename(filename), mexportOp(exportOp)
  {};

  int operator()(std::string sessionKey) {
    return exportCommands(sessionKey, moldSessionId, mfilename, mexportOp);
  }
};


boost::shared_ptr<Options>
makeExportOpt(string pgName,
            boost::function1<void, IMS_Data::ExportType>& fExportType,
            string& dietConfig) {

  boost::shared_ptr<Options> opt(new Options(pgName));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

    // All cli options
   opt->add("exportType,t",
            "the type to export\n"
            "1 for SHELL",
            CONFIG,
            fExportType);

  return opt;
}


int main (int argc, char* argv[]){

  /******* Parsed value containers ****************/
  string dietConfig;
  string oldSessionId;
  string filename;

   /********** EMF data ************/
  IMS_Data::ExportOp exportOp;

  /******** Callback functions ******************/
  boost::function1<void, IMS_Data::ExportType> fExportType (boost::bind(&IMS_Data::ExportOp::setExportType,boost::ref(exportOp),_1));


  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeExportOpt(argv[0], fExportType, dietConfig);

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
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "[options] oldSessionId filename");

  //call of the api function
  ExportFunc exportFunc(oldSessionId, filename, exportOp);
  return GenericCli().run(exportFunc, dietConfig, argc, argv);

}

