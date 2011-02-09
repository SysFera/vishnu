/**
 * \file IMSVishnuException.hpp
 * \brief This file defines the IMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __IMSVISHNUEXCEPTION__
#define __IMSVISHNUEXCEPTION__

#include "SystemException.hpp"

class IMSVishnuException: public SystemException{
private :
protected:
public:
  /**
   * \brief Default constructor
   * \fn IMSVishnuException()
   */
  IMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  IMSVishnuException(const IMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  IMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~IMSVishnuException()
   */
  ~IMSVishnuException() throw(){};
  
};

#endif // IMSVishnuException
