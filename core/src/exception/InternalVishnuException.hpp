/**
 * \file InternalVishnuException.hpp
 * \brief This file defines the internal sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __INTERNALVISHNUEXCEPTION__
#define __INTERNALVISHNUEXCEPTION__

#include "SystemException.hpp"

static const int INNBERR = 5;

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
   * \fn UMSVishnuException()
   */
  InternalVishnuException(InternalVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  InternalVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~InternalVishnuException()
   */
  ~InternalVishnuException() throw(){};
  /**
   * \brief Function to get the generic message in the Internal part depending on the inherited mval integer
   * \fn getMsg
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // InternalVishnuException
