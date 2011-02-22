/**
 * \file UMSVishnuException.hpp
 * \brief This file defines the UMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __UMSVISHNUEXCEPTION__
#define __UMSVISHNUEXCEPTION__

#include "UserException.hpp"

// static const int UNBERR = 5;  A QUOI CA SERT??

static const int ERRCODE_UNKNOWN_USER = 20;
static const int ERRCODE_INCORRECT_USERID = 21;
static const int ERRCODE_INCORRECT_PASSWORD = 22;
static const int ERRCODE_UNKNOWN_CLOSURE_MODE = 23;
static const int ERRCODE_INCORRECT_TIMEOUT = 24;
static const int ERRCODE_SESSIONKEY_NOT_FOUND = 26;

static const int ERRCODE_SESSIONKEY_EXPIRED = 27;
static const int ERRCODE_USERID_EXISTING = 28;
static const int ERRCODE_INVALID_MAIL_ADRESS = 29;
static const int ERRCODE_SESSION_INCOMPATIBILITY = 30; // *** TO CHECK
static const int ERRCODE_NO_ADMIN = 31;
static const int ERRCODE_UNKNOWN_OPTION = 32;
static const int ERRCODE_UNKNOWN_SESSION_ID = 33;
static const int ERRCODE_UNKNOWN_MACHINE = 34;
static const int ERRCODE_UNKNOWN_USERID = 35;
static const int ERRCODE_LOCAL_ACCOUNT_EXIST = 36;
static const int ERRCODE_UNKNOWN_LOCAL_ACCOUNT = 37;
static const int ERRCODE_COMMAND_RUNNING = 38;
static const int ERRCODE_MACHINE_EXISTING = 39;
// static const int INCORRECT_DATE_OPTION = 40; // CLI
static const int ERRCODE_SAVE_CONFIG_ERROR = 41; 
static const int ERRCODE_RESTORE_CONFIG_ERROR = 42;
// static const int USERID_REQUIRED = 43;   // CLI
// static const int MACHINEID_REQUIRED = 44;  // CLI
// static const int UNKNOWN_SESSION_OPTION = 45; // CLI
static const int ERRCODE_UNKNOWN_MACHINE_FOR_SESSION = 46;    // *** TO CHECK
static const int ERRCODE_TEMPORARY_PASSWORD = 47;
static const int ERRCODE_USER_LOCKED = 48;
static const int ERRCODE_MACHINE_LOCKED = 49;
static const int ERRCODE_USER_ALREADY_LOCKED = 50;
static const int ERRCODE_MACHINE_ALREADY_LOCKED = 51;
static const int ERRCODE_UNUSABLE_MACHINE = 52;
static const int ERRCODE_INCOMPATIBLE_USER_SESSION = 53;      // *** TO CHECK


class UMSVishnuException: public UserException{

public:
  /**
   * \brief Default constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(UMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(int msg, std::string msgComp = "");
  /**
   * \brief Default constructor
   * \fn ~UMSVishnuException()
   */
  ~UMSVishnuException()throw (){};
  /**
   * \brief Function to initialize all UMS generic messages
   * \fn void initMsg()
   */
  void
  initMsg();
};

#endif // UMSVishnuException
