/**
 * \file IMSMapper.hpp
 * \brief Mapper for the IMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __IMSMAPPER__HH__
#define __IMSMAPPER__HH__

#include <map>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"


using namespace std;

/**
 * \brief To export commands
 */
const int VISHNU_EXPORT     = 1;
/**
 * \brief Get current metric
 */
const int VISHNU_GET_CUR    = 2;
/**
 * \brief Get history
 */
const int VISHNU_GET_HIST   = 3;
/**
 * \brief Get processes
 */
const int VISHNU_GET_PROC   = 4;
/**
 * \brief Set system info
 */
const int VISHNU_SET_SYSINF = 5;
/**
 * \brief Set threshold
 */
const int VISHNU_SET_THRESH = 6;
/**
 * \brief Get threshold
 */
const int VISHNU_GET_THRESH = 7;
/**
 * \brief Define user id format
 */
const int VISHNU_DEFINE_UID = 8;
/**
 * \brief Define machine id format
 */
const int VISHNU_DEFINE_MID = 9;
/**
 * \brief Define task id format
 */
const int VISHNU_DEFINE_TID = 10;
/**
 * \brief Define file transfer id format
 */
const int VISHNU_DEFINE_FID = 11;
/**
 * \brief Load shedding
 */
const int VISHNU_LOADSHED   = 12;
/**
 * \brief Set update frequency
 */
const int VISHNU_SET_FREQ   = 13;
/**
 * \brief Get Update frequency
 */
const int VISHNU_GET_FREQ   = 14;
/**
 * \brief Stop process
 */
const int VISHNU_STOP       = 15;
/**
 * \brief Restart a process
 */
const int VISHNU_RESTART    = 16;
/**
 * \brief Get system info
 */
const int VISHNU_GET_SYSINF = 17;
/**
 * \brief Define authentication system id
 */
const int VISHNU_DEFINE_AID = 18;

/**
 * \class IMSMapper
 * \brief Mapper implementation for the IMS module
 */
class IMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \fn IMSMapper(MapperRegistry* reg, string name)
   * \param reg Registry to register
   * \param name Name to use to register
   */
  IMSMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   * \fn IMSMapper()
   */
  IMSMapper();
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
   * \fn  virtual int code(const string& cmd, unsigned int code = 0)
   * \param cmd The string to add
   * \param code If 0, the code is created, otherwize it is the key in the map to add the string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const string& cmd, unsigned int code = 0);
  /**
   * \brief To get, from a coded string, the cli like command that made it
   * \fn virtual string decode (const string& msg);
   * \param msg The coded string
   * \return The cli like command
   */
  virtual string
  decode (const string& msg);

protected:
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeExp(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeCur(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeHist(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeProc(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeSetSys(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeGetThre(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeSetThre(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeFid(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeMid(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeUid(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeTid(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeAid(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeLoad(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeGetF(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeSetF(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeStop(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeRestart(vector<unsigned int> separator, const string& msg);
  /**
   * \brief To decode the close call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual string
  decodeGetSys(vector<unsigned int> separator, const string& msg);

private:
};


#endif
