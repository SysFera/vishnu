/**
 * \file UserException.hpp
 * \brief This file defines class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __USEREXCEPTION__
#define __USEREXCEPTION__

#include "VishnuException.hpp"

// RESERVED CODES FROM 10 TO 19
static const int ERRCODE_INVALID_PARAM = 10;

class UserException :
  public VishnuException {
public:

private :
protected :
  /**
   * \brief Function to initialize all the generic messages
   * \fn initMsg()
   */
  void
  initMsg();

public :

  /**
   * \brief Default constructor
   * \fn UserException()
   */
  UserException();
  /**
   * \brief Copy constructor
   * \fn UserException(UserException e)
   */
  UserException(const UserException& e);
  /**
   * \brief Constructor
   * \fn UserException(int msg, string msgComp = "")
   */
  UserException(int msg, std::string msgComp = "");
  /**
   * \brief Default destructor
   * \fn ~VishnuException()
   */
  virtual ~UserException() throw(){};
  /**
   * \brief Function to get the exception type in string
   * \fn virtual std::string getTypeS()
   * \return Returns the type of the exception
   */
  std::string
  getTypeS() const;
  /**
   * \brief To get the generic message
   * \fn std::string getMsg()
   * \return Returns the message
   */
  std::string
  getMsg() const;
};

#endif // VISHNUEXCEPTION
