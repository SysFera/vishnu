#include "Options.hh"

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

/* Default constructor. */
Configuration::Configuration() {
  pgName = "unknown";
}

/* Standard constructor. Get the program name as parameter. */
Configuration::Configuration(const std::string& pgName) {
  this->pgName = pgName;
}

/* Generic program configuration accessors. */
const string& Configuration::getConfigFile() const {
  return configFile;
}

const std::string& Configuration::getPgName() const {
  return pgName;
}

void Configuration::setConfigFile(const string& configFile) {
  this->configFile = configFile;
}

/* Standard Options constructor. Get a pointer on a config object, and
 * the command line arguments. */
Options::Options(Configuration* config, int argc, char* argv[], char* envp[]) {
  unsigned int j = 0;
  
  this->config = config;
  
  for (int i=1; i<argc; ++i) {
    string curArg(argv[i]);
    if (curArg.find("--")==0) {
      if (i==argc-1) singleArgs.push_back(curArg);
      else {
        string nextArg(argv[i+1]);
        if (nextArg.find('-')==0) singleArgs.push_back(curArg);
        else {
          arguments[curArg]=nextArg;
          ++i;
        }
      }
    } else {
      if (curArg.find('-')==0) {
        string::iterator it = curArg.begin();
        while (it!=curArg.end()) {
          flags.push_back(*it++);
        }
      } else {
          params[params.size()]=curArg;
      }
    }
  }  
  if (envp==NULL) return;
  while (envp[j]!=NULL) {
    string curEnv(envp[j++]);
    size_t pos = curEnv.find('=');
    if (pos==string::npos)
      singleEnvs.push_back(curEnv);
    else {
      string key=curEnv.substr(0, pos);
      string value=curEnv.substr(pos+1);
      environment[key]=value;
    }
  }
}

/* Callback functions definitions. */
/* Define the callback function for a parameter that need an argument.
 * These parameters must start with "--".
 */
void Options::setOptCallback(const string& arg, optCallback callBack) {
  optCallbacks[arg]=callBack;
}

/* Define the callback function for an environment variable. */
void Options::setEnvCallback(const string& arg, optCallback callBack) {
  envCallbacks[arg]=callBack;
}

/* Define the callback function for "lonely" parameters (not preceded by
 * a "--" argument. */
void Options::setParamCallback(unsigned int idx, optCallback callBack) {
  paramCallbacks[idx]=callBack;
}

/* Define the callback function for a parameter that don't take argument.
 * These parameters must start with a single '-'.
 */
void Options::setFlagCallback(const char flag, optCallback callBack) {
  flagCallbacks[flag]=callBack;
}

/* Process the options using the callback functions. */
void Options::processOptions() {
  map<string,string>::const_iterator i;
  list<string>::const_iterator j;
  unsigned int k;
  list<char>::const_iterator l;
  optCallback callback;
  
  for (l=flags.begin(); l!=flags.end(); ++l) {
    if (flagCallbacks.find(*l)!=flagCallbacks.end()) {
      string flag;
      flag+=*l;
      callback = flagCallbacks.find(*l)->second;
      callback(flag, config);
    } else {
      if (*l!='-')
        cerr << "Warning: -" << *l << " flag unknown" << endl;
    }
  }
  for (i=arguments.begin(); i!=arguments.end(); ++i) {
    if (optCallbacks.find(i->first)!=optCallbacks.end()) {
      callback = optCallbacks.find(i->first)->second;
      callback(i->second, config);
    } else {
      cerr << "Warning: " << i->first << " unknown option" << endl;
    }
  }
  for (j=singleArgs.begin(); j!=singleArgs.end(); ++j) {
    if (optCallbacks.find(*j)!=optCallbacks.end()) {
      callback = optCallbacks.find(*j)->second;
      callback("", config);
    } else {
      cerr << "Warning: " << *j << " unknown option" << endl;
    }
  }
  for (k=0;k<params.size();++k) {
    if (paramCallbacks.find(k)!=paramCallbacks.end()) {
      callback = paramCallbacks[k];
      callback(params[k], config);
    } else {
      cerr << "Warning: argument " << params[k] << " ignored" << endl; 
    }
  }
}

/* Process the environment variables using the defined callback functions. */
void Options::processEnv() {
  map<string, string>::const_iterator i;
  list<string>::const_iterator j;
  optCallback callback;
  
  for (i=environment.begin(); i!=environment.end(); ++i) {
    if (envCallbacks.find(i->first)!=envCallbacks.end()) {
      callback = envCallbacks.find(i->first)->second;
      callback(i->second, config);
    }
  }
  for (j=singleEnvs.begin(); j!=singleEnvs.end(); ++j) {
    if (envCallbacks.find(*j)!=envCallbacks.end()) {
      callback = envCallbacks.find(*j)->second;
      callback(*j, config);
    }
  }
}

/* Simple utility function:
 * Replace the '=' character with a white space.
 * Used to extract information with istringstream.
 */
int cut(int c) {
  if (c == '=') return ' ';
  return c;
}

ConfigFile::ConfigFile() {}

ConfigFile::ConfigFile(const string& path) {
  parseFile(path);
}

void ConfigFile::parseFile(const std::string& path) {
  ifstream file(path.c_str());
  unsigned int l = 0;
  
  if (!file.is_open())
    throw runtime_error("Can't open "+path);
  
  while (!file.eof()) {
    char buffer[1024];
    string line, key, value;
    string::iterator it;
    istringstream is;
    size_t pos;
    
    l++;
    file.getline(buffer, 1024);
    line = buffer;
    /* Remove comments. */
    pos = line.find('#');
    line = line.substr(0, pos);
    
    /* Remove white spaces. */
    while ((pos=line.find(' '))!=string::npos)
      line.erase(pos, 1);
    while ((pos=line.find('\t'))!=string::npos)
      line.erase(pos, 1);
    
    /* Empty line => continue. */
    if (line=="") continue;
    
    /* Cut the line on '=' chararcter. */
    transform(line.begin(), line.end(), line.begin(), cut);
    /* Extract key,value */
    is.str(line);
    is >> key >> value;
    if (value=="")
      cerr << "Warning: \"" << key << "\" has no value! (l." << l
      << ")" << endl;
    if (attributes.find(key)!=attributes.end())
      cerr << "Warning: Multiple values for the attribute " << key
      << " (l." << l << ")" << endl;
    /* Transform to lower case. */
    transform(key.begin(), key.end(), key.begin(), ::tolower);

    attributes[key]=value;    
  }
}
const string& ConfigFile::getAttr(const string& key) {
  return attributes[key];
}
