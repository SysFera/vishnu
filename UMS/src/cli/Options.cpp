#include "Options.hpp"

#include <string>
#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

using namespace std;
namespace po=boost::program_options;
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

// standard Options constructor. Get a string (command name)

Options:: Options(const std::string& pgName )   : generic_options("Generic Options"),
                                                  config_options("Configuration"),
                                                  env_options("Environment variables"),
                                                  hidden_options("Hidden Options"),
                                                  conf(new Configuration(pgName)){


    generic_options.add_options()
      ("help,h", "produce help message")
      ;


  }

/* Standard Options constructor. Get a pointer on a config object*/
Options::Options(boost::shared_ptr<Configuration> otherConf):conf(otherConf),
                                                             generic_options("Generic Options"),
                                                             config_options("Configuration"),
                                                             env_options("Environment variables"),
                                                             hidden_options("Hidden Options"){

    generic_options.add_options()
      ("help,h", "produce help message")
      ;


  }

boost::shared_ptr<Configuration>
Options::getConfiguration()const{

  return this->conf;



}

void Options::setGroup (const po::options_description& tmp_options,
    const Group_type& group){


  switch(group){

    case GENERIC:

      generic_options.add(tmp_options);
      break;

    case CONFIG:

      config_options.add(tmp_options);
      break;

    case ENV:

      env_options.add(tmp_options);
      break;

    case HIDDEN:

      hidden_options.add(tmp_options);
      break;

    default:

      cout << "unknown option group " << endl;

  }
}






void
Options::add(const std::string& name,
    const std::string& desc, const Group_type& group ){


  po::options_description tmp_options;

  tmp_options.add_options()(name.c_str(),desc.c_str());

  setGroup(tmp_options,group);

}



// set the position of the option

void Options::setPosition(const std::string& name, int pos ){
  this->position.add(name.c_str(),pos);
}

/* Parse the command line. */
void Options::parse_cli(int ac, char* av[]) {

  po::options_description cmdline_options;

  cmdline_options.add(generic_options)
    .add(config_options)
    .add(env_options)
    .add(hidden_options);

  store(po::command_line_parser(ac, av)
      .options(cmdline_options).positional(position)
      .run(), vm);
}

// Parse the configuration file
void Options::parse_cfile() {

  po::options_description cfile_options;

  cfile_options.add(config_options)
    .add(hidden_options)
    .add(env_options);

  ifstream ifs(conf->getConfigFile().c_str());

  store(parse_config_file(ifs,cfile_options), vm);

}

// Parse environement variable

void Options::parse_env(const func1& name_mapper) {

  store(parse_environment(env_options,name_mapper), vm);
}




// Notify user-defined functions

void Options::notify (){

  po::notify(vm);

}

// check if key exists in the map
//
int Options::count(const std::string& key)const{

  return (vm.count(key));
}


//  print visible options


std::ostream & operator<< (std::ostream & os, const Options & opt){


  po::options_description visible("Allowed options");

  visible.add(opt.generic_options)
    .add(opt.config_options)
    .add(opt.env_options);

  os << visible << endl;

  return os;
}

// The destructor

Options::~Options(){}
