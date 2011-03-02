/**
 * \file UMSMapper.hpp
 * \brief Mapper for the UMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __UMSMAPPER__HH__
#define __UMSMAPPER__HH__

#include <map>
#include <vector>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"
#include "utilVishnu.hpp"

using namespace std;

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
  UMSMapper(MapperRegistry* reg, string name);
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
  getCommand(const int& key,string& command);

  /**
   * \brief To get the key corresponding to a command
   * \fn virtual int getKey(const string& command, int& key)
   * \param command The 'key' to use
   * \param key OUT, the key corresponding to the command
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const string& command, int& key);

  /**
   * \brief Function to add an element to code
   * \fn  virtual int code(const string& cmd, unsigned int code = 0);
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const string& cmd, unsigned int code = 0);

  /**
   * \brief To end a command, delete it from the map and get its value
   * \fn virtual string finalize(int key);
   * \param key The entry in the map to get an remove
   * \return The coded value of the command
   */
  virtual string
  finalize(int key);

  /**
   * \brief To get, from a coded string, the cli like command that made it
   * \fn virtual string decode (const string& msg);
   * \param msg The coded string
   * \return The cli like command
   */
  virtual string
  decode (const string& msg);


protected:
private:

  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \fn string decodeClose(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
  decodeClose(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeAddUser(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add user command
   */
  string
  decodeAddUser(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeUpUser(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update user command
   */
  string
  decodeUpUser(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeDelUser(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete user command
   */
  string
  decodeDelUser(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeChangePwd(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like change password command
   */
  string
  decodeChangePwd(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeResetPwd(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like reset password command
   */
  string
  decodeResetPwd(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeAddAcc(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add account command
   */
  string
  decodeAddAcc(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeUpAcc(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update account command
   */
  string
  decodeUpAcc(vector<int> separator, const string& msg);
  
  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeDelAcc(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete account command
   */
  string
  decodeDelAcc(vector<int> separator, const string& msg);
  
  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeSaveConf(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like save configuration command
   */
  string
  decodeSaveConf(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeRestoreConf(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like crestore configuration command
   */
  string
  decodeRestoreConf(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeAddM(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add machine command
   */
  string
  decodeAddM(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeUpM(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update machine command
   */
  string
  decodeUpM(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeDelM(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete machine command
   */
  string
  decodeDelM(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListAcc(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list account command
   */
  string
  decodeListAcc(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListM(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list machine command
   */
  string
  decodeListM(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListH(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list history command
   */
  string
  decodeListH(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListOp(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list option command
   */
  string
  decodeListOp(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListUser(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list user command
   */
  string
  decodeListUser(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeListSession(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list session command
   */
  string
  decodeListSession(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeConfDefaultOp(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like configure default option command
   */
  string
  decodeConfDefaultOp(vector<int> separator, const string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \fn string decodeConfOp(vector<int> separator, const string& msg)
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like configure option command
   */
  string
  decodeConfOp(vector<int> separator, const string& msg);

  /**
   * \brief To find all the separator in the string s and fill the vector with their position in the string
   * \fn void findSeparator(const string& s, vector<int>& vec)
   * \param s The string to find the separator
   * \param vec A vector containing the position of the separator in the string s (OUT)
   * \return The cli like close command
   */
  void
  findSeparator(const string& s, vector<int>& vec);
  
  /**
   * \brief To deserialize the serial string, make a user objet, and get the values in the same string
   * \fn string getU(string serial)
   * \param serial The serialized object
   * \return A string containing the user fields in the right order
   */
  string
  getU(string serial);

};


#endif
