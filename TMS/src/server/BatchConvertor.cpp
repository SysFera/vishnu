#include <cstring>
#include <boost/algorithm/string.hpp>

#include "BatchConvertor.hpp"
#include "UserException.hpp"

namespace ba=boost::algorithm;

BatchConvertor::BatchConvertor(const int batchType, 
                               const std::string& genericJob): 
  mbatchType(batchType), mgenericJob(genericJob) 
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
    mendScript = "# @ queue";

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
    mconversionTable[cpuTime]           = "#PBS cput ";
    mconversionTable[loadLevelerSec]    = "";
    mconversionTable[commandSec]        = "";
    mconversionTable[torqueSec]         = "";
    mendScript="";

  } else {
   std::cerr << "Unknown Batch ID " << std::endl;
 }

}

void BatchConvertor::initializeTableOfSymbols() {

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

/*doit envoyer -1 en cas d'erreur*/
int BatchConvertor::parseFile(std::string& errorMessage) {

   /*To initialize the table of symbols*/
   initializeTableOfSymbols();

   std::istringstream imgenericJob(mgenericJob);

   std::string line;
   size_t pos, key_pos;
   std::string key, value, linebuf;
 
   std::vector<std::string>::iterator iter; 
   int numline = 0;

   while(!imgenericJob.eof()) {

     std::istringstream is;
     getline(imgenericJob, line, '\n');

     numline +=1;

     /*search # character*/
     pos = line.find('#');
    
     if(pos == string::npos) {

      linebuf=ba::erase_all_copy(line," ");

       if(linebuf.empty()) {
         continue;
       }
       
       else {

         key="command_sec";

         mjobDescriptor.push_back (make_pair(key,line));

       }
     }
   
     line = line.erase(0, pos); // supprime les tous les caractères jusqu'au # (exclu)

     // traiter les directives spécifiques ici

// LOADLEVELER
     if(ba::starts_with( ba::erase_all_copy(line," "),"#@")){
    
       if (mbatchType==LOADLEVELER){
         
         key="loadleveler_sec"; 
         mjobDescriptor.push_back (make_pair(key,line));
       }
       else{
         /*if(ba::ends_with(line, "\\")) {
           std::cout << "line11=" << line << endl;
         };*/
         continue;
       }
     } 
// TORQUE
     if(ba::starts_with(line,"#PBS")){
   
       if (mbatchType==TORQUE){
         
         key="torque_sec";

         mjobDescriptor.push_back (make_pair(key,line));
       }
       else {

         continue;
       }
     } 
    // SHEBANG
     if(ba::starts_with(line,"#!")){

        key="command_sec";
         mjobDescriptor.push_back (make_pair(key,line));
       }


     /*remove % character*/
     if(!ba::starts_with(line,"#%")){
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

       ba::erase_all(line, " ");//while((pos=line.find(' '))!=string::npos) line.erase(pos, 1);
       
       if(line=="") continue; 
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

     if(key=="") continue;

     /*transform to lower case */
     std::string key_tolower(key);
     std::transform(key.begin(), key.end(), key_tolower.begin(), ::tolower);
     iter = std::find(mtableOfSymbols.begin(), mtableOfSymbols.end(), key_tolower);
     if(iter==mtableOfSymbols.end()) {
      cerr << std::endl;
      cerr << "Error : Invalid argument " << key_tolower << " at line " << numline << " in your script mgenericJob" << std::endl;
      ostringstream os_error;
      os_error << "Error : Invalid argument " << key_tolower << " at line " << numline << " in your script mgenericJob" << std::endl;
      errorMessage = os_error.str();
      return -1;
    }

    value = line.substr(pos+1);

    while((value[0]==' ')) value.erase(0,1);    

    /*transform to lower case */
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
     
    mjobDescriptor.push_back(make_pair(key,value));

  }

 return 0;
}

std::string BatchConvertor::getJobDescriptor() {

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

bool BatchConvertor::scriptIsGeneric() {

  std::string modifiedScript(mgenericJob);
  std::transform(mgenericJob.begin(), mgenericJob.end(), modifiedScript.begin(), ::tolower);  
  modifiedScript = ba::erase_all_copy(modifiedScript," ");
  if(modifiedScript.find("#%"+prefix)!=std::string::npos){
    return true;
  } 
  return false;
}

BatchConvertor::~BatchConvertor() {
}

boost::shared_ptr<BatchConvertor>  
vishnuJobConvertor(const int batchType, 
                   const std::string& genericJob) {

  boost::shared_ptr< BatchConvertor> JobDesc(new BatchConvertor(batchType, genericJob));
  string parse_error ;
  if(JobDesc->parseFile(parse_error)==-1) {
    std::string errorMessage = "Can't generate batch genericJob script \n"+parse_error ;
    throw UserException(ERRCODE_INVALID_PARAM, errorMessage);
  } ;
  return JobDesc;
}

