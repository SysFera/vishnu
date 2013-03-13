/**
 * \file UMSMapper.hpp
 * \brief Mapper for the UMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __UMSMAPPER__HH__
#define __UMSMAPPER__HH__

#include <string>
#include <vector>

#include "Mapper.hpp"

class MapperRegistry;

/**
 * \brief Connection key
 */
const int VISHNU_CONNECT                  = 1 ;
/**
 * \brief Reconnection key
 */
const int VISHNU_RECONNECT 	     	  = 2 ;
/**
 * \brief Closure key
 */
const int VISHNU_CLOSE 	     	          = 3 ;
/**
 * \brief Add user key
 */
const int VISHNU_ADD_VISHNU_USER   	  = 4 ;
/**
 * \brief Update user key
 */
const int VISHNU_UPDATE_VISHNU_USER 	  = 5 ;
/**
 * \brief Delete user key
 */
const int VISHNU_DELETE_VISHNU_USER 	  = 6 ;
/**
 * \brief Change password key
 */
const int VISHNU_CHANGE_PASSWORD   	  = 7 ;
/**
 * \brief Reset password key
 */
const int VISHNU_RESET_PASSWORD    	  = 8 ;
/**
 * \brief Add account key
 */
const int VISHNU_ADD_LOCAL_ACCOUNT        = 9 ;
/**
 * \brief Update account key
 */
const int VISHNU_UPDATE_LOCAL_ACCOUNT     = 10;
/**
 * \brief Delete account key
 */
const int VISHNU_DELETE_LOCAL_ACCOUNT     = 11;
/**
 * \brief Save configuration key
 */
const int VISHNU_SAVE_CONFIGURATION       = 12;
/**
 * \brief Restore configuration key
 */
const int VISHNU_RESTORE_CONFIGURATION    = 13;
/**
 * \brief Add machine key
 */
const int VISHNU_ADD_MACHINE 	          = 14;
/**
 * \brief Update machine key
 */
const int VISHNU_UPDATE_MACHINE           = 15;
/**
 * \brief Delete machine key
 */
const int VISHNU_DELETE_MACHINE           = 16;
/**
 * \brief List account key
 */
const int VISHNU_LIST_LOCAL_ACCOUNT       = 17;
/**
 * \brief List machine key
 */
const int VISHNU_LIST_MACHINE 	          = 18;
/**
 * \brief List history key
 */
const int VISHNU_LIST_HISTORY_CMD         = 19;
/**
 * \brief List option key
 */
const int VISHNU_LIST_OPTIONS 	          = 20;
/**
 * \brief List user key
 */
const int VISHNU_LIST_USERS 	          = 21;
/**
 * \brief List session key
 */
const int VISHNU_LIST_SESSIONS 	          = 22;
/**
 * \brief Configure default option key
 */
const int VISHNU_CONFIGURE_DEFAULT_OPTION = 23;
/**
 * \brief Configure option key
 */
const int VISHNU_CONFIGURE_OPTION	  = 24;
/**
 * \brief Add authentication system
 */
const int VISHNU_ADD_AUTHSYS 	          = 25;
/**
 * \brief Update authentication system
 */
const int VISHNU_UPDATE_AUTHSYS          = 26;
/**
 * \brief Delete authentication system
 */
const int VISHNU_DELETE_AUTHSYS           = 27;
/**
 * \brief List authentication system
 */
const int VISHNU_LIST_AUTHSYS             = 28;
/**
 * \brief Add authentification account
 */
const int VISHNU_ADD_AUTHACC 	          = 29;
/**
 * \brief Update authentication account
 */
const int VISHNU_UPDATE_AUTHACC           = 30;
/**
 * \brief Delete authentication account
 */
const int VISHNU_DELETE_AUTHACC           = 31;
/**
 * \brief List authentication account
 */
const int VISHNU_LIST_AUTHACC       = 32;


/**
 * \class UMSMapper
 * \brief Mapper implementation for the UMS module
 */
class UMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \fn UMSMapper(MapperRegistry* reg, string name)
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  UMSMapper(MapperRegistry* reg, std::string name);
  /**
   * \brief Default constructor
   * \fn UMSMapper()
   */
  UMSMapper();
  /**
   * \brief Destructor
   * \fn ~UMSMapper()
   */
  ~UMSMapper();
  /**
   * \brief To register a mapper in the list of active mapper
   * \fn virtual int registerMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper();

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \fn virtual int unregisterMapper()
   * \return 0 on success, an error code otherwise
   */
  virtual int
  unregisterMapper();

  /**
   * \brief To get the command corresponding to a key
   * \fn virtual int getCommand(const int& key,string& command)
   * \param key The key to use
   * \param command OUT, the command corresponding to the key
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const int& key, std::string& command);

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey(const std::string& command, int& key)
   * \param command The 'key' to use
   * \param key OUT, the key corresponding to the command
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const std::string& command, int& key);

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const std::string& cmd, unsigned int code = 0)
   * \param cmd The std::string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const std::string& cmd, unsigned int code = 0);

  /**
   * \brief To get, from a coded string, the cli like command that made it
   * \fn virtual std::string decode (const std::string& msg);
   * \param msg The coded string
   * \return The cli like command
   */
  virtual std::string
  decode (const std::string& msg);


protected:

  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \fn std::string decodeClose(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeClose(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeAddUser(vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add user command
   */
  virtual std::string
  decodeAddUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeUpUser(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update user command
   */
  virtual std::string
  decodeUpUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeDelUser(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete user command
   */
  virtual std::string
  decodeDelUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add authentication system call sequence of the string returned by finalize
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add user command
   */
  virtual std::string
  decodeAddAuthSys(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the update authentication system call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update user command
   */
  virtual std::string
  decodeUpAuthSys(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the delete authentication account call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete user command
   */
  virtual std::string
  decodeDelAuthSys(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add authentication account call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add user command
   */
  virtual std::string
  decodeAddAuthAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the update authentication account call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update user command
   */
  virtual std::string
  decodeUpAuthAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the delete authentication account call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete user command
   */
  virtual std::string
  decodeDelAuthAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeChangePwd(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like change password command
   */
  virtual std::string
  decodeChangePwd(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeResetPwd(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like reset password command
   */
  virtual std::string
  decodeResetPwd(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeAddAcc(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add account command
   */
  virtual std::string
  decodeAddAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeUpAcc(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update account command
   */
  virtual std::string
  decodeUpAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeDelAcc(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete account command
   */
  virtual std::string
  decodeDelAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeSaveConf(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like save configuration command
   */
  virtual std::string
  decodeSaveConf(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeRestoreConf(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like crestore configuration command
   */
  virtual std::string
  decodeRestoreConf(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeAddM(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add machine command
   */
  virtual std::string
  decodeAddM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeUpM(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update machine command
   */
  virtual std::string
  decodeUpM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeDelM(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete machine command
   */
  virtual std::string
  decodeDelM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListAcc(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list account command
   */
  virtual std::string
  decodeListAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the list authentication system call sequence of the string returned by finalize
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list account command
   */
  virtual std::string
  decodeListAuthSys(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the list authentication account call sequence of the string returned by finalize
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list account command
   */
  virtual std::string
  decodeListAuthAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListM(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list machine command
   */
  virtual std::string
  decodeListM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListH(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list history command
   */
  virtual std::string
  decodeListH(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListOp(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list option command
   */
  virtual std::string
  decodeListOp(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListUser(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list user command
   */
  virtual std::string
  decodeListUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeListSession(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list session command
   */
  virtual std::string
  decodeListSession(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeConfDefaultOp(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like configure default option command
   */
  virtual std::string
  decodeConfDefaultOp(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn std::string decodeConfOp(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like configure option command
   */
  virtual std::string
  decodeConfOp(std::vector<unsigned int> separator, const std::string& msg);


  /**
   * \brief To deserialize the serial string, make a user objet, and get the values in the same string
   * \fn std::string getU(std::string serial)
   * \param serial The serialized object
   * \return A string containing the user fields in the right order
   */
  std::string
  getU(std::string serial);

  /**
   *\brief To deserialize the serial string, make a user objet, and get the values in the same string
   *\fn std::string getUupdate(std::string serial)
   *\param serial The serialized object
   *\return A string containing the user fields in the right order
   */
  std::string
  getUupdate(std::string serial);
private:
};


#endif
