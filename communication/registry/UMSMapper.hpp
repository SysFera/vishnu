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

enum MapperService {
  VISHNU_CONNECT = 1,
  VISHNU_RECONNECT,
  VISHNU_CLOSE,
  VISHNU_ADD_VISHNU_USER,
  VISHNU_UPDATE_VISHNU_USER,
  VISHNU_DELETE_VISHNU_USER,
  VISHNU_CHANGE_PASSWORD,
  VISHNU_RESET_PASSWORD,
  VISHNU_ADD_LOCAL_ACCOUNT,
  VISHNU_UPDATE_LOCAL_ACCOUNT,
  VISHNU_DELETE_LOCAL_ACCOUNT,
  VISHNU_ADD_MACHINE,
  VISHNU_UPDATE_MACHINE,
  VISHNU_DELETE_MACHINE,
  VISHNU_LIST_LOCAL_ACCOUNT,
  VISHNU_LIST_MACHINE,
  VISHNU_LIST_HISTORY_CMD,
  VISHNU_LIST_USERS,
  VISHNU_LIST_SESSIONS,
  VISHNU_ADD_AUTHSYS,
  VISHNU_UPDATE_AUTHSYS,
  VISHNU_DELETE_AUTHSYS,
  VISHNU_LIST_AUTHSYS,
  VISHNU_ADD_AUTHACC,
  VISHNU_UPDATE_AUTHACC,
  VISHNU_DELETE_AUTHACC,
  VISHNU_LIST_AUTHACC,
  VISHNU_DEFINE_UID,
  VISHNU_DEFINE_MID,
  VISHNU_DEFINE_TID,
  VISHNU_DEFINE_FID,
  VISHNU_DEFINE_AID,
  VISHNU_DEFINE_WID,
  /**
   * \brief To export commands
   */
  VISHNU_EXPORT
};




/**
 * \class UMSMapper
 * \brief Mapper implementation for the UMS module
 */
class UMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \param reg Registry to be linked to
   * \param name Name of the mapper
   */
  UMSMapper(MapperRegistry* reg, std::string name);
  /**
   * \brief Default constructor
   */
  UMSMapper();
  /**
   * \brief Destructor
   */
  ~UMSMapper();
  /**
   * \brief To register a mapper in the list of active mapper
   * \return 0 on success, an error code otherwise
   */
  virtual int
  registerMapper();

  /**
   * \brief To unregister a mapper in the list of active mapper
   * \return 0 on success, an error code otherwise
   */
  virtual int
  unregisterMapper();

  /**
   * \brief To get the command corresponding to a key
   * \param key The key to use
   * \param command OUT, the command corresponding to the key
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const int& key, std::string& command);

  /**
   * \brief To get the key corresponding to a command
   * \param command The 'key' to use
   * \param key OUT, the key corresponding to the command
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getKey(const std::string& command, int& key);

  /**
   * \brief Function to add an element to code
   * \param cmd The std::string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const std::string& cmd, unsigned int code = 0);

  /**
   * \brief To get, from a coded string, the cli like command that made it
   * \param msg The coded string
   * \return The cli like command
   */
  virtual std::string
  decode (const std::string& msg);


protected:

  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeClose(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add user command
   */
  virtual std::string
  decodeAddUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update user command
   */
  virtual std::string
  decodeUpUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
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
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like change password command
   */
  virtual std::string
  decodeChangePwd(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like reset password command
   */
  virtual std::string
  decodeResetPwd(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add account command
   */
  virtual std::string
  decodeAddAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update account command
   */
  virtual std::string
  decodeUpAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete account command
   */
  virtual std::string
  decodeDelAcc(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like add machine command
   */
  virtual std::string
  decodeAddM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like update machine command
   */
  virtual std::string
  decodeUpM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like delete machine command
   */
  virtual std::string
  decodeDelM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
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
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list machine command
   */
  virtual std::string
  decodeListM(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list history command
   */
  virtual std::string
  decodeListH(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list user command
   */
  virtual std::string
  decodeListUser(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the add user call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like list session command
   */
  virtual std::string
  decodeListSession(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeFid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeMid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeUid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeTid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeAid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeWid(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeExp(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To deserialize the serial string, make a user objet, and get the values in the same string
   * \param serial The serialized object
   * \return A string containing the user fields in the right order
   */
  std::string
  getU(std::string serial);

  /**
   *\brief To deserialize the serial string, make a user objet, and get the values in the same string
   *\param serial The serialized object
   *\return A string containing the user fields in the right order
   */
  std::string
  getUupdate(std::string serial);
private:
};


#endif
