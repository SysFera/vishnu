#ifndef CONNECTUTILS__HH
#define CONNECTUTILS__HH


#include<iostream>
#include<boost/shared_ptr.hpp>

#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "UMS_Data.hpp"


class Options;


template <typename userIdType>

boost::shared_ptr<Options> makeConnectOptions(std::string pgName,userIdType& userId,std::string& dietConfig){

 boost::shared_ptr<Options> opt(new Options(pgName));


        opt->add("dietConfig,c",
                "The diet config file",
            ENV,
            dietConfig);

    opt->add("userId,u",
                "represents the VISHNU user identifier",
            HIDDEN,
            userId,
            1);

//    opt->setPosition("userId",pos);



    return opt;


}



#endif



