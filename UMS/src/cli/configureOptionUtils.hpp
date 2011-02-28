#ifndef CONFIGUREOPTIONUTILS__HH
#define CONFIGUREOPTIONUTILS__HH

#include "api_ums.hpp"

#include<iostream>
#include<boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"

#include "Options.hpp"



typedef  boost::function1<void,std::string> StringcallBackType;

typedef  boost::function2<int,const std::string&,const UMS_Data::OptionValue&> ConfigureCallBackType;

void makeConfigureOptions(boost::shared_ptr<Options>,std::string&,
		                                            StringcallBackType&,StringcallBackType& );

void commonConfigure(boost::shared_ptr<Options>, int , char*[] , const ConfigureCallBackType&);


/*
 * \struct env_name_mapper
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */


struct env_name_mapper{

  std::string operator ()(std::string name){

    std::string result= "";

                if (name=="VISHNU_CONFIG_FILE") {

                        result="dietConfig";
                }

    return result;
  }
};





#endif



