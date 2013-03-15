/**
 * \file InternalVishnuException.hpp
 * \brief This file defines the internal sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __INTERNALVISHNUEXCEPTION__
#define __INTERNALVISHNUEXCEPTION__

#include <string>
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
   */
  InternalVishnuException();
  /**
   * \brief Copy constructor
   */
  InternalVishnuException(InternalVishnuException& e);
  /**
   * \brief Constructor
   */
  InternalVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   */
  ~InternalVishnuException() throw(){};
  /**
   * \brief Function to get the generic message in the Internal part depending on the inherited mval integer
   * \return Returns the corresponding generic message
   */
  std::string getMsg();



};

#endif // InternalVishnuException
