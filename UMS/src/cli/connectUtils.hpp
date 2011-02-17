#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;

typedef boost::function1<void,UMS_Data::SessionCloseType>  fClosePolicyType;

boost::shared_ptr<Options> makeConnectOptions(std::string,std::string&,int ,std::string&);

