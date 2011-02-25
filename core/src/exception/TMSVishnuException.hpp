/**
 * \file TMSVishnuException.hpp
 * \brief This file defines the TMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __TMSVISHNUEXCEPTION__
#define __TMSVISHNUEXCEPTION__

#include "UserException.hpp"

static const int TNBERR = 5;

class TMSVishnuException: public UserException{
private :
protected:
public:
  /**
   * \brief Default constructor
   * \fn TMSVishnuException()
   */
  TMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(TMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~TMSVishnuException()
   */
  ~TMSVishnuException() throw (){};
  
};

#endif // TMSVishnuException
