/**
 * \file InternalVishnuException.hpp
 * \brief This file defines the internal sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __INTERNALVISHNUEXCEPTION__
#define __INTERNALVISHNUEXCEPTION__

#include "SystemException.hpp"

static const int INNBERR = 5;

/**
 * \brief This class represents the exception that are internal
 * \class InternalVishnuException
 */
class InternalVishnuException: public SystemException{
private :
protected:
public:
  /**
   * \brief Default constructor
   * \fn InternalVishnuException()
   */
  InternalVishnuException();
  /**
   * \brief Copy constructor
   * \fn InternalVishnuException(InternalVishnuException& e)
   */
  InternalVishnuException(InternalVishnuException& e);
  /**
   * \brief Constructor
   * \fn InternalVishnuException(int msg, std::string msgComp)
   */
  InternalVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~InternalVishnuException() throw()
   */
  ~InternalVishnuException() throw(){};
  /**
   * \brief Function to get the generic message in the Internal part depending on the inherited mval integer
   * \fn std::string getMsg()
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // InternalVishnuException
