/**
 * \file IMSVishnuException.hpp
 * \brief This file defines the IMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __IMSVISHNUEXCEPTION__
#define __IMSVISHNUEXCEPTION__

#include "UserException.hpp"

/**
 * \class IMSVishnuException
 * \brief Specialisation of the user exception for the IMS module
 */
class IMSVishnuException: public UserException{
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
