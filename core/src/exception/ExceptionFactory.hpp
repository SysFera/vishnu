/**
 * \file ExceptionFactory.hpp
 * \brief This file defines the factory for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __EXCEPTIONFACTORY__
#define __EXCEPTIONFACTORY__

#include "utilVishnu.hpp"
#include "VishnuException.hpp"

using namespace std;

/**
 * \brief This class represents the factory for all the exception
 * \class ExceptionFactory
 */
class ExceptionFactory {
public:
  /**
   * \brief Function to create an exception whose type depend on the code
   * \fn VishnuException* create(int code, string msg)
   * \param code : The generic error code
   * \param msg : The complementary message
   * \return The exception created
   */
  VishnuException* create(int code, string msg);
  /**
   * \brief Default constructor
   * \fn ExceptionFactory()
   */
  ExceptionFactory();
  /**
   * \brief Default destructor
   * \fn ~ExceptionFactory()
   */
  ~ExceptionFactory();
protected :
private :
};

#endif // class exception factory
