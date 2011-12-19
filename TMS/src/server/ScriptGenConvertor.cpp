/**
 * \file ScriptGenConvertor.cpp
 * \brief This file contains the VISHNU ScriptGenConvertor class.
 * \authors Ibrahima Cisse (ibrahima.cisse@sysfera.com) and 
 * Daouda Traore (daouda.traore@sysfera.com)
 * \date May 2011
 */

#include <cstring>
#include <boost/algorithm/string.hpp>

#include "ScriptGenConvertor.hpp"
#include "UserException.hpp"

namespace ba=boost::algorithm;

/**
 * \brief Constructor 
 * \param batchType the type of the batch scheduler
 * \param scriptGenContent the generic script to convert
 */
ScriptGenConvertor::ScriptGenConvertor(const int batchType, 
                                       const std::string& scriptGenContent): 
  mbatchType(batchType), mscriptGenContent(scriptGenContent) 
{
  if(mbatchType==LOADLEVELER) {

    mconversionTable[group]             = "# @ group=";
    mconversionTable[jobName]           = "# @ job_name=";
    mconversionTable[jobOutput]         = "# @ output=";
    mconversionTable[jobError]          = "# @ error=";
    mconversionTable[jobWallClockLimit] = "# @ wall_clock_limit=";
    mconversionTable[workingDir]        = "# @ initialdir=";
    mconversionTable[notify_user]       = "# @ notification=complete \n # @ notify_user=";
    mconversionTable[nodes]             = "# @ job_type=parallel \n # @ node=";
    mconversionTable[queue]             = "# @ class="; 
    mconversionTable[priority]          = "# @ user_priority=";
    mconversionTable[mem]               = "# @ data_limit=";
    mconversionTable[cpuTime]           = "# @ cpu_limit= ";
    mconversionTable[loadLevelerSec]    = "";
    mconversionTable[commandSec]        = "";
    mconversionTable[torqueSec]         = "";
    mendScript                          = "# @ queue";

  } else if(mbatchType==TORQUE) {

    mconversionTable[group]             = "#PBS -W group_list=";
    mconversionTable[jobName]           = "#PBS -N ";
    mconversionTable[jobOutput]         = "#PBS -o ";
    mconversionTable[jobError]          = "#PBS -e ";
    mconversionTable[jobWallClockLimit] = "#PBS -l walltime=";
    mconversionTable[workingDir]        = "#PBS -d ";
    mconversionTable[notify_user]       = "#PBS -m e -M ";
    mconversionTable[nodes]             = "#PBS -l nodes=";
    mconversionTable[queue]             = "#PBS -q ";
    mconversionTable[priority]          = "#PBS -p " ;
    mconversionTable[mem]               = "#PBS -l mem=";
    mconversionTable[cpuTime]           = "#PBS -l cput ";
    mconversionTable[loadLevelerSec]    = "";
    mconversionTable[commandSec]        = "";
    mconversionTable[torqueSec]         = "";
    mendScript="";

  } else if(mbatchType==SLURM) {

    mconversionTable[group]             = "#SBATCH --gid=";
    mconversionTable[jobName]           = "#SBATCH -J ";
    mconversionTable[jobOutput]         = "#SBATCH -o ";
    mconversionTable[jobError]          = "#SBATCH -e ";
    mconversionTable[jobWallClockLimit] = "#SBATCH -t ";
    mconversionTable[workingDir]        = "#SBATCH -D ";
    mconversionTable[notify_user]       = "#SBATCH --mail-type=END --mail-user=";
    mconversionTable[nodes]             = "#SBATCH -N ";
    mconversionTable[queue]             = "#SBATCH -p ";
    mconversionTable[priority]          = "#SBATCH --nice=" ;
    mconversionTable[mem]               = "#SBATCH --mem=";
    mconversionTable[cpuTime]           = "#SBATCH --mincpus=";
    mconversionTable[slurmSec]          = "";
    mconversionTable[commandSec]        = "";
    mconversionTable[torqueSec]         = "";
    mendScript="";

  } else {
    std::cerr << "Unknown Batch type " << std::endl;
  }

}

/**
 * \brief Function to return the converted script 
 */
void 
ScriptGenConvertor::initializeTableOfSymbols() {

  mtableOfSymbols.push_back(group);
  mtableOfSymbols.push_back(jobName);
  mtableOfSymbols.push_back(jobOutput);
  mtableOfSymbols.push_back(jobError);
  mtableOfSymbols.push_back(jobWallClockLimit);
  mtableOfSymbols.push_back(workingDir);
  mtableOfSymbols.push_back(notify_user);
  mtableOfSymbols.push_back(queue);
  mtableOfSymbols.push_back(nodes);
  mtableOfSymbols.push_back(mem);
  mtableOfSymbols.push_back(cpuTime);
  mtableOfSymbols.push_back(priority);
  mtableOfSymbols.push_back(loadLevelerSec);
  mtableOfSymbols.push_back(torqueSec);
  mtableOfSymbols.push_back(commandSec);
}

/**
 * \brief Function to parse the generic script 
 * \param errorMessage is the message of the errors occured during the parsing
 * \return 0 if success, -1 if if failure 
 */
int 
ScriptGenConvertor::parseFile(std::string& errorMessage) {

  /*To initialize the table of symbols*/
  initializeTableOfSymbols();

  std::istringstream imscriptGenContent(mscriptGenContent);

  std::string line;
  std::string key;
  std::string value;
  std::string linebuf;
  size_t pos;

  std::vector<std::string>::iterator iter; 
  int numline = 0;
  std::string tmpLine="";
  size_t escapePos;
  bool escapeFound = false;

  while(!imscriptGenContent.eof()) {

    getline(imscriptGenContent, line);

    numline +=1;
    //Treating of the escape character int the script content
    if(ba::ends_with(ba::erase_all_copy(line, " "),"\\")){
      escapePos = line.rfind("\\");
      if(escapePos!=std::string::npos) {
        tmpLine += line.substr(0, escapePos);
        escapeFound = true;
        continue;
      }
    }

    if(escapeFound) {
      tmpLine +=line;
      line = tmpLine;
      escapeFound = false;
      tmpLine = "";
    }

    /*search # character*/
    pos = line.find('#');

    if(pos == string::npos) {
      linebuf=ba::erase_all_copy(line," ");
      if(linebuf.empty()) {
        continue;
      } else {
        key=commandSec;
        mjobDescriptor.push_back (make_pair(key,line));
      }
    }

    line = line.erase(0, pos); // erase all character until # (excluded)

    // treats the specific directives here
    // LOADLEVELER
    if(ba::starts_with( ba::erase_all_copy(line," "),"#@")){
      if (mbatchType==LOADLEVELER){
        key=loadLevelerSec;; 
        mjobDescriptor.push_back (make_pair(key,line));
      } else{
        continue;
      }
    } 
    // TORQUE
    if(ba::starts_with(line,"#PBS")){
      if (mbatchType==TORQUE){
        key=torqueSec;
        mjobDescriptor.push_back (make_pair(key,line));
      } else {
        continue;
      }
    }
    // SLURM
    if(ba::starts_with(line,"#SBATCH")){
      if (mbatchType==SLURM){
        key=slurmSec;
        mjobDescriptor.push_back (make_pair(key,line));
      } else {
        continue;
      }
    } 

    // SHEBANG
    if(ba::starts_with(line,"#!")){
      key=commandSec;
      mjobDescriptor.push_back (make_pair(key,line));
    }

    /*remove % character*/
    if(!ba::starts_with(ba::erase_all_copy(line," "),"#%")){
      continue;
    }

    pos = line.find('%');

    if(pos == string::npos) {
      continue;
    }
    line = line.erase(0, pos+1);

    /* Extract key, value*/
    pos = line.find('=');
    if(pos == string::npos) {
      ba::erase_all(line, " ");
      if (line.empty()) {
        continue;
      }
      /*transform to lower case */
      std::string line_tolower(line);
      std::transform(line.begin(), line.end(), line_tolower.begin(), ::tolower);
      iter = std::find(mtableOfSymbols.begin(), mtableOfSymbols.end(), line_tolower);
      if(iter==mtableOfSymbols.end()) {
        cerr << std::endl;
        cerr << "Error : Invalid argument " << line_tolower << " at line " << numline << " in your script file" << std::endl;
        ostringstream os_error;
        os_error << "Error : Invalid argument " << line_tolower << " at line " << numline << " in your script file" << std::endl;
        errorMessage = os_error.str();
        return -1;
      }
      continue;
    }

    key = line.substr(0, pos);
    ba::erase_all(key, " ");

    if (key.empty()) {
      continue;
    }

    /*transform to lower case */
    std::string key_tolower(key);
    std::transform(key.begin(), key.end(), key_tolower.begin(), ::tolower);
    iter = std::find(mtableOfSymbols.begin(), mtableOfSymbols.end(), key_tolower);
    if(iter==mtableOfSymbols.end()) {
      cerr << std::endl;
      cerr << "Error : Invalid argument " << key_tolower << " at line " << numline << " in your script file" << std::endl;
      ostringstream os_error;
      os_error << "Error : Invalid argument " << key_tolower << " at line " << numline << " in your script file" << std::endl;
      errorMessage = os_error.str();
      return -1;
    }

    value = line.substr(pos+1);

    while((value[0]==' ')) {
      value.erase(0,1);
    }

    /*transform to lower case */
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    mjobDescriptor.push_back(make_pair(key,value));

  }

  return 0;
}

/**
 * \brief Function to return the converted script 
 */
std::string 
ScriptGenConvertor::getConvertedScript() {

  std::string result ;
  std::string key, value;
  std::vector< pair<std::string, std::string> >::const_iterator iter;

  for(iter = mjobDescriptor.begin(); iter!=mjobDescriptor.end(); ++iter) {

    key =  iter->first;
    value = iter->second;
    result += mconversionTable[key]  + value + "\n";
  }

  return result+mendScript;
}

/**
 * \brief Function to check if the script is generic
 */
bool
ScriptGenConvertor::scriptIsGeneric() {

  std::string modifiedScript(mscriptGenContent);
  std::transform(mscriptGenContent.begin(), mscriptGenContent.end(), modifiedScript.begin(), ::tolower);  
  modifiedScript = ba::erase_all_copy(modifiedScript," ");
  if(modifiedScript.find("#%"+prefix)!=std::string::npos){
    return true;
  } 
  return false;
}

/**
 * \brief Destructor
 */
ScriptGenConvertor::~ScriptGenConvertor() {
}

/**
 * \brief Function to return the generic script convertor 
 * \param batchType The type of the batch scheduler
 * \param scriptGenContent The content of the script to convert
 * \return The generic script convertor
 */
boost::shared_ptr<ScriptGenConvertor>  
vishnuScriptGenConvertor(const int batchType, 
                         const std::string& scriptGenContent) {

  boost::shared_ptr< ScriptGenConvertor> scriptGenConvertor(new ScriptGenConvertor(batchType, scriptGenContent));
  string parse_error ;
  if(scriptGenConvertor->parseFile(parse_error)==-1) {
    std::string errorMessage = "Can't generate this generic script content \n"+parse_error ;
    throw UserException(ERRCODE_INVALID_PARAM, errorMessage);
  } ;
  return scriptGenConvertor;
}

