#ifndef CONFIGUREOPTIONUTILS__HH
#define CONFIGUREOPTIONUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>
#include <boost/function.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;


typedef  boost::function1<void,std::string> StringcallBackType;

boost::shared_ptr<Options> makeConfigureOptions(std::string,std::string&,
		                                            StringcallBackType&,StringcallBackType& );




#endif



