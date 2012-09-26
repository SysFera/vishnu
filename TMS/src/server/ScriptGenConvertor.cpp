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

    mconversionTable[group]                = "# @ group=";
    mconversionTable[workingDir]           = "# @ initialdir=";
    mconversionTable[jobName]              = "# @ job_name=";
    mconversionTable[jobOutput]            = "# @ output=";
    mconversionTable[jobError]             = "# @ error=";
    mconversionTable[jobWallClockLimit]    = "# @ wall_clock_limit=";
    mconversionTable[cpuTime]              = "# @ cpu_limit= ";
    mconversionTable[nbCpu]                = "# @ "; //special case
    mconversionTable[nbNodesAndCpuPerNode] = "# @ ";//special case
    mconversionTable[mem]                  = "# @ data_limit=";//a voir
    mconversionTable[mailNotification]     = "# @ notification=";//special case
    mconversionTable[mailNotifyUser]       = "# @ notify_user=";
    mconversionTable[queue]                = "# @ class="; 
   
    mconversionTable[loadLevelerSec]       = "";
    mconversionTable[commandSec]           = "";
    mconversionTable[torqueSec]            = "";
    mendScript                             = "# @ queue";

  } else if(mbatchType==TORQUE) {

    mconversionTable[group]                = "#PBS -W group_list=";
    mconversionTable[workingDir]           = "#PBS -d ";
    mconversionTable[jobName]              = "#PBS -N ";
    mconversionTable[jobOutput]            = "#PBS -o ";
    mconversionTable[jobError]             = "#PBS -e ";
    mconversionTable[jobWallClockLimit]    = "#PBS -l walltime=";
    mconversionTable[cpuTime]              = "#PBS -l cput=";
    mconversionTable[nbCpu]                = "## PBS -l "; //special case
    mconversionTable[nbNodesAndCpuPerNode] = "#PBS -l "; //special case
    mconversionTable[mem]                  = "#PBS -l mem=";
    mconversionTable[mailNotification]     = "#PBS -m "; //special case
    mconversionTable[mailNotifyUser]       = "#PBS -M ";
    mconversionTable[queue]                = "#PBS -q ";
    
    mconversionTable[loadLevelerSec]       = "";
    mconversionTable[commandSec]           = "";
    mconversionTable[torqueSec]            = "";
    mendScript="";

  }else if(mbatchType==PBSPRO) {
    
    mconversionTable[group]                = "#PBS -W group_list=";
    mconversionTable[workingDir]           = "#PBS -d ";
    mconversionTable[jobName]              = "#PBS -N ";
    mconversionTable[jobOutput]            = "#PBS -o ";
    mconversionTable[jobError]             = "#PBS -e ";
    mconversionTable[jobWallClockLimit]    = "#PBS -l walltime=";
    mconversionTable[cpuTime]              = "#PBS -l cput=";
    mconversionTable[nbCpu]                = "## PBS -l "; //special case
    mconversionTable[nbNodesAndCpuPerNode] = "#PBS -l "; //special case
    mconversionTable[mem]                  = "#PBS -l mem=";
    mconversionTable[mailNotification]     = "#PBS -m "; //special case
    mconversionTable[mailNotifyUser]       = "#PBS -M ";
    mconversionTable[queue]                = "#PBS -q ";
    
    mconversionTable[loadLevelerSec]       = "";
    mconversionTable[commandSec]           = "";
    mconversionTable[torqueSec]            = "";
    mendScript="";
    
  }else if(mbatchType==SLURM) {

    mconversionTable[group]                = "#SBATCH --gid=";
    mconversionTable[workingDir]           = "#SBATCH -D ";
    mconversionTable[jobName]              = "#SBATCH -J ";
    mconversionTable[jobOutput]            = "#SBATCH -o ";
    mconversionTable[jobError]             = "#SBATCH -e ";
    mconversionTable[jobWallClockLimit]    = "#SBATCH -t ";
    mconversionTable[cpuTime]              = "#SBATCH -t ";
    mconversionTable[nbCpu]                = "#SBATCH --mincpus=";
    mconversionTable[nbNodesAndCpuPerNode] = "#SBATCH "; //spacial case
    mconversionTable[mem]                  = "#SBATCH --mem=";
    mconversionTable[mailNotification]     = "#SBATCH --mail-type=";//special case; 
    mconversionTable[mailNotifyUser]       = "#SBATCH --mail-user=";
    mconversionTable[queue]                = "#SBATCH -p ";
    
    mconversionTable[slurmSec]             = "";
    mconversionTable[commandSec]           = "";
    mconversionTable[torqueSec]            = "";
    mendScript="";

  } else if(mbatchType==LSF) {
     
    mconversionTable[group]                = "#BSUB -G ";
    mconversionTable[workingDir]           = "#BSUB -cwd ";
    mconversionTable[jobName]              = "#BSUB -J ";
    mconversionTable[jobOutput]            = "#BSUB -o ";
    mconversionTable[jobError]             = "#BSUB -e ";
    mconversionTable[jobWallClockLimit]    = "#% -vishnuWaillClockLimit="; //spacial case: treated in LSFParser
    mconversionTable[cpuTime]              = "#BSUB -c ";
    mconversionTable[nbCpu]                = "#% -vishnuCpu=";  //spacial case: treated in LSFParser
    mconversionTable[nbNodesAndCpuPerNode] = "#% -vishnuNbNodesAndCpuPerNode="; //spacial case: treated in LSFParser
    mconversionTable[mem]                  = "#BSUB -M ";
    mconversionTable[mailNotification]     = "#% -vishnuMailNofication="; //special case; treated in LSFParser 
    mconversionTable[mailNotifyUser]       = "#BSUB -u ";
    mconversionTable[queue]                = "#BSUB -q ";
    
    mconversionTable[lsfSec]             = "";
    mconversionTable[commandSec]           = "";
    mendScript="";

  } else if (mbatchType==SGE){

    mconversionTable[group]                = "";
    mconversionTable[workingDir]           = "#$ -wd ";
    mconversionTable[jobName]              = "#$ -N ";
    mconversionTable[jobOutput]            = "#$ -o ";
    mconversionTable[jobError]             = "#$ -e ";
    mconversionTable[jobWallClockLimit]    = "#$ -l s_rt=";
    mconversionTable[cpuTime]              = "#$ -l s_cpu=";
    mconversionTable[nbCpu]                = ""; 
    mconversionTable[nbNodesAndCpuPerNode] = ""; 
    mconversionTable[mem]                  = "#$ -l s_vmem=";
    mconversionTable[mailNotification]     = "#$ -m "; //special case
    mconversionTable[mailNotifyUser]       = "#$ -M ";
    mconversionTable[queue]                = "#$ -q ";
    
    mconversionTable[sgeSec]               = "";
    mconversionTable[commandSec]           = "";
    mconversionTable[torqueSec]            = "";
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
  mtableOfSymbols.push_back(workingDir);
  mtableOfSymbols.push_back(jobName);
  mtableOfSymbols.push_back(jobOutput);
  mtableOfSymbols.push_back(jobError);
  mtableOfSymbols.push_back(jobWallClockLimit);
  mtableOfSymbols.push_back(cpuTime);
  mtableOfSymbols.push_back(nbCpu);
  mtableOfSymbols.push_back(nbNodesAndCpuPerNode);
  mtableOfSymbols.push_back(mem);
  mtableOfSymbols.push_back(mailNotification);  
  mtableOfSymbols.push_back(mailNotifyUser);
  mtableOfSymbols.push_back(queue);
  
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

    // treats the specific directives here
    //LSF 
    if(ba::starts_with( ba::erase_all_copy(line," "),"#BSUB")){
      if (mbatchType==LSF){
        key=lsfSec;; 
        mjobDescriptor.push_back (make_pair(key,line));
      } else{
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
        ostringstream os_error;
        os_error << "Error : Invalid argument " << line << " at line " << numline << " in your script file" << std::endl;
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
      ostringstream os_error;
      os_error << "Error : Invalid argument " << key << " at line " << numline << " in your script file" << std::endl;
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
  std::string torqueNodes;
  bool torqueNodeIsAdd = false;
  std::vector< pair<std::string, std::string> >::const_iterator iter;
  for(iter = mjobDescriptor.begin(); iter!=mjobDescriptor.end(); ++iter) {

    key =  iter->first;
    value = iter->second;

    //Special case
    if(key.compare(nbNodesAndCpuPerNode)==0) {

      if(!value.empty()){
        if(*(value.begin())=='\"'){
          value.replace(value.begin(), value.begin()+1, "");
        }
        if(*(value.end()-1)=='\"'){
          value.replace(value.end()-1, value.end(), "");
        } 
      }

      size_t posNbNodes = value.find(":");
      if(posNbNodes!=std::string::npos) {
        std::string nbNodes = value.substr(0, posNbNodes);
        std::string cpuPerNode = value.substr(posNbNodes+1);

        if(mbatchType==LOADLEVELER) {
          value = " node="+nbNodes+"\n";
          value += "# @ tasks_per_node=1\n";
          value += "# @ tasks_affinity = core(1) \n";
          value += " @ cpus_per_node = "+cpuPerNode+"\n";
        } else if(mbatchType==TORQUE) {
          value = " nodes="+nbNodes+":ppn="+cpuPerNode;
        } else if(mbatchType==SLURM) {
          value = " --nodes="+nbNodes+"\n#SBATCH --mincpus="+cpuPerNode; 
        }
      }
    }
    //Special case
    if(mbatchType==LOADLEVELER && key.compare(nbCpu)==0) {
      value ="# @ tasks_per_node = 1 \n";
      value +="# @ tasks_affinity = core(1) \n";
      value +="# @ cpus_per_node = "+nbCpu+"\n";
    }

    //Special case
    if(mbatchType==TORQUE && key.compare(nbCpu)==0) {
      std::istringstream imscriptGenContent(mscriptGenContent);
      std::string line;
      bool ppnNotDefined=true;;
      while(!imscriptGenContent.eof()) {
        getline(imscriptGenContent, line);
        size_t pos = line.find("#PBS");
        if(pos!=std::string::npos) {
          size_t posL = line.find("-l", pos);
          if(posL!=std::string::npos){
            if(line.find("nodes=", pos)!=std::string::npos) {
              line = line.substr(posL+2);
              ppnNotDefined = false;
              findAndReplace(":ppn=", value, line);
              torqueNodes += "#PBS -l "+line+"\n";
            }
          }
        }
      }
      if(ppnNotDefined){
        value = " nodes=1:ppn="+value;
      }
    }

    //Special case
    if(mbatchType==TORQUE && key.compare(commandSec)==0){
      if(!torqueNodes.empty() && !torqueNodeIsAdd){
        result += torqueNodes;
        torqueNodeIsAdd = true;
      }
    }

    //Special case
    if(key.compare(mailNotification)==0) {

      bool notificationIsNotValid = false;
      if(mbatchType==LOADLEVELER) {
        if(value.compare("BEGIN")==0) {
          value= "start";
        } else if(value.compare("END")==0) {
          value = "complete";
        } else if(value.compare("ERROR")==0) {
          value = "error";
        } else if(value.compare("ALL")==0) {
          value= "always";
        } else {
          notificationIsNotValid = true;
        }
      } else if(mbatchType==TORQUE) {
        if(value.compare("BEGIN")==0) {
          value= "b";
        } else if(value.compare("END")==0) {
          value = "e";
        } else if(value.compare("ERROR")==0) {
          value = "a";
        } else if(value.compare("ALL")==0) {
          value= "abe";
        } else {
          notificationIsNotValid = true;
        }
      } else if(mbatchType==SLURM) {
        if(value.compare("BEGIN")==0) {
          value= "BEGIN";
        } else if(value.compare("END")==0) {
          value = "END";
        } else if(value.compare("ERROR")==0) {
          value = "FAIL";
        } else if(value.compare("ALL")==0) {
          value= "ALL";
        } else {
          notificationIsNotValid = true;
        }
      } else if(mbatchType==SGE){
        if(value.compare("BEGIN")==0) {
          value= "b";
        } else if(value.compare("END")==0) {
          value = "e";
        } else if(value.compare("ERROR")==0) {
          value = "a";
        } else if(value.compare("ALL")==0) {
          value= "abe";
        } else {
          notificationIsNotValid = true;
        }
      }

      if(notificationIsNotValid) {
        throw UserException(ERRCODE_INVALID_PARAM, value+" is an invalid notification type:"+" consult the vishnu user manuel");
      }
    }

    result += mconversionTable[key]  + value + "\n";
  }

  return result+mendScript;
}

/**
 * \brief Function to insert some additional (ppn+nbCpuStr) content in the string str
 * \param ppn A part of the content to insert
 * \param nbCpuStr An other part of the content to insert
 * \param str The string to modify
 */
void
ScriptGenConvertor::findAndReplace(const std::string& ppn, 
                                   const std::string& nbCpuStr, 
                                   std::string& str){
  
  size_t pos = str.find(ppn);
  while(pos!=std::string::npos) {
    std::string oldPPNValue;
    size_t posFirstChar = str.find_first_not_of("0123456789", pos+ppn.size());
    if(posFirstChar!=std::string::npos) {
      oldPPNValue = str.substr(pos+ppn.size(), posFirstChar-(pos+ppn.size()));
    } else {
      oldPPNValue =  str.substr(pos+ppn.size());
    }
    str.replace(pos+ppn.size(), oldPPNValue.size(), nbCpuStr);
    pos = str.find(ppn, pos+1);
  }

  char delim = '+';
  size_t begin = 0;
  size_t end = str.find(delim);
  std::string tmp;
  if(end==std::string::npos) {
    findAndInsert(ppn, ppn+nbCpuStr, begin, end, str);
  }
  while(end!=std::string::npos) {
    findAndInsert(ppn, ppn+nbCpuStr, begin, end, str);
    begin = end+1;
    end = str.find(delim, begin);
    //last token
    if(end==std::string::npos){
      findAndInsert(ppn, ppn+nbCpuStr, begin, end, str);
    }
  }
}

/**
 * \brief Function to insert some additional content (newValue)
 * \param oldValue oldValue to replace 
 * \param newValue new value to insert
 * \param begin The begin position of the substring in str
 * \param end The end position of the substring in str
 * \param str The string to modify
 */
void
ScriptGenConvertor::findAndInsert(const std::string& oldValue,
                                  const std::string& newValue,
                                  const size_t& begin,
                                  size_t& end,
                                  std::string& str) {

  std::string substr;
  if(end!=std::string::npos) {
   substr = str.substr(begin, end-begin);
  } else {
   substr = str.substr(begin);
  }
  if(substr.find(oldValue)==std::string::npos) {
    str.insert(begin+substr.size(), newValue);
    if(end!=std::string::npos) {
      end += newValue.size();
    }
  }
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

