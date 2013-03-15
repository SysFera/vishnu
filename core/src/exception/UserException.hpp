/**
 * \file UserException.hpp
 * \brief This file defines class for the Vishnu exceptions coming from the user.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __USEREXCEPTION__
#define __USEREXCEPTION__

#include <string>
#include "VishnuException.hpp"

// RESERVED CODES FROM 10 TO 19
static const int ERRCODE_INVALID_PARAM = 10;
static const int ERRCODE_FILENOTFOUND = 11;
static const int ERRCODE_CONFIGNOTFOUND = 12;
static const int ERRCODE_CLI_ERROR_NO_SESSION =13;
static const int ERRCODE_CLI_ERROR_MISSING_PARAMETER =14;
static const int ERRCODE_CLI_ERROR_COMMUNICATION = 15;
static const int ERRCODE_CLI_ERROR_RUNTIME = 16;
/**
 * \brief This class represents the exception due to a user misuse
 * \class UserException
 */
class UserException :  public VishnuException {
public :
  /**
   * \brief Default constructor
   */
  UserException();
  /**
   * \brief Copy constructor
   */
  UserException(const UserException& e);
  /**
   * \brief Constructor
   */
  UserException(int msg, std::string msgComp = "");
  /**
   * \brief Default destructor
   */
  virtual ~UserException() throw(){};
  /**
   * \brief Function to get the exception type in string
   * \return Returns the type of the exception
   */
  std::string
  getTypeS() const;
  /**
   * \brief To get the generic message
   * \return Returns the message
   */
  std::string
  getMsg() const;

protected :
  /**
   * \brief Function to initialize all the generic messages
   */
  void
  initMsg();
};

#endif // VISHNUEXCEPTION
