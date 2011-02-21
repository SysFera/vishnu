/**
 * \file UMSVishnuException.hh
 * \brief This file defines the UMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __UMSVISHNUEXCEPTION__
#define __UMSVISHNUEXCEPTION__

#include "SystemException.hh"

static const int UNBERR = 5;

static const int UNKNOWN_USER = 20; 
static const int INCORRECT_USERID = 21; 
static const int INCORRECT_PASSWORD = 22; 
static const int UNKNOWN_CLOSURE_MODE = 23; 
static const int INCORRECT_TIMEOUT = 24; 
static const int UMS_SERVER_NOT_AVAILABLE = 25; 
static const int SESSIONKEY_NOT_FOUND = 26; 
static const int SESSIONKEY_EXPIRED = 27; 
static const int USERID_EXISTING = 28; 
static const int INVALID_MAIL_ADRESS = 29; 
static const int SESSION_INCOMPATIBILITY = 30; 
static const int NO_ADMIN = 31; 
static const int UNKNOWN_OPTION = 32; 
static const int UNKNOWN_SESSION_ID = 33; 
static const int UNKNOWN_MACHINE = 34; 
static const int UNKNOWN_USERID = 35; 
static const int LOCAL_ACCOUNT_EXIST = 36; 
static const int UNKNOWN_LOCAL_ACCOUNT = 37; 
static const int COMMAND_RUNNING = 38; 
static const int MACHINE_EXISTING = 39; 
static const int INCORRECT_DATE_OPTION = 40; 
static const int SAVE_CONFIG_ERROR = 41; 
static const int RESTORE_CONFIG_ERROR = 42; 
static const int USERID_REQUIRED = 43; 
static const int MACHINEID_REQUIRED = 44; 
static const int UNKNOWN_SESSION_OPTION = 45; 
static const int UNKNOWN_MACHINE_FOR_SESSION = 46; 
static const int TEMPORARY_PASSWORD = 47; 
static const int USER_LOCKED = 48; 
static const int MACHINE_LOCKED = 49; 
static const int USER_ALREADY_LOCKED = 50; 
static const int MACHINE_ALREADY_LOCKED = 51; 
static const int UNUSABLE_MACHINE = 52; 
static const int INCOMPATIBLE_USER_SESSION = 53;

class UMSVishnuException: public SystemException{
private :
protected:
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
