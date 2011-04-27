/**
 * \file IMSVishnuException.hh
 * \brief This file defines the IMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __IMSVISHNUEXCEPTION__
#define __IMSVISHNUEXCEPTION__

#include "UserException.hpp"

// Definitions of IMS specific error codes
static const int ERRCODE_INVPROCESS = 301;


class IMSVishnuException: public UserException{

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
  IMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default constructor
   * \fn ~IMSVishnuException()
   */
  ~IMSVishnuException() throw (){};

  /**
   * \brief Function to initialize all UMS generic messages
   * \fn void initMsg()
   */
  void
  initMsg();


};

#endif // IMSVishnuException
