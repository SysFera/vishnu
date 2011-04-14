#include "cliUtil.hpp"

using namespace std;
/**
 * \brief helper function to display help about a specific command 
 * \param opt: describes all options allowed by the command
 * \param signature: defines the usage of the command 
 */
void 
helpUsage (const Options& opt,const string& signature){

  cout << "\nUsage: " << opt.getConfiguration()->getPgName()<<" " << signature <<"\n\n";
  cout <<opt<< endl;
}


/**
 * \brief helper function to display error about a specific command 
 * \param cli   :The name of the command
 * \param errMsg: The error message to display 
 * \param err   : The error type 
 */
void 
errorUsage (const string & cli,const string& errMsg,const ErrorType& err){

  cerr << cli<<": "<<errMsg <<endl;

  if(err==PARAMERROR){
    cerr << "To get help, try <<"<< cli << " -h >>"<< endl;
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
usage (const Options & opt,const std::string& mess,const std::string& eWhat){

  int res=0;

  if(opt.count("help")){

    helpUsage(opt,mess);
  }
  else{

    errorUsage(opt.getConfiguration()->getPgName(),eWhat);

    res=1;
  }

  return res;
}

/**
 * \brief A helper function which check if VISHNU_CONFIG_FILE is set
 *\param opt: describes all options allowed by the command
 *\exception raise a runtime exception if the VISHNU_CONFIG_FILE is not set
 */


void 
checkVishnuConfig(const Options & opt){
  if (opt.count("dietConfig")==0){
    throw runtime_error( "Set the VISHNU_CONFIG_FILE in your environment variable");
  }

}
