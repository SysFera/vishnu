/**
 * \file UMSVishnuException.hh
 * \brief This file defines the UMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __UMSVISHNUEXCEPTION__
#define __UMSVISHNUEXCEPTION__

#include "SystemException.hh"

static const int UNBERR = 5;

class UMSVishnuException: public SystemException{
private :
protected:
public:
  /**
   * \brief Default constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(UMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~UMSVishnuException()
   */
  ~UMSVishnuException()throw (){};
  
};

#endif // UMSVishnuException
