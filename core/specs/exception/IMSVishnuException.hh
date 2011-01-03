/**
 * \file IMSVishnuException.hh
 * \brief This file defines the IMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __IMSVISHNUEXCEPTION__
#define __IMSVISHNUEXCEPTION__

#include "SystemException.hh"

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
  IMSVishnuException(IMSVishnuException& e);
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
