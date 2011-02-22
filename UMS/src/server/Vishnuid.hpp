#include <iostream>

#ifndef _VISHNUID_H_
#define _VISHNUID_H_
/**
* \class Vishnuid
* \brief The Vishnuid class encapsulates the global vishnu id
*/
class Vishnuid {
  public:
    static std::string mvishnuid;
    static std::string mformatiduser;
    static std::string mformatidmachine;
    static std::string mformatidjob;
    static std::string mformatidfiletransfer;
};

#endif//VISHNUID