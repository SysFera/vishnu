/**
 * \file FMSMapper.hpp
 * \brief Mapper for the FMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * and Daouda Traore (daouda.traore@sysfera.com)
 * \date May 2011
 */

#ifndef __FMSMAPPER__HH__
#define __FMSMAPPER__HH__

#include <map>

#include "Mapper.hpp"
#include "MapperRegistry.hpp"


using namespace std;

/**
 * \brief Create file key
 */
const int VISHNU_CREATE_FILE              = 1;
/**
 * \brief Create dir key
 */
const int VISHNU_CREATE_DIR               = 2;
/**
 * \brief Remove file key
 */
const int VISHNU_REMOVE_FILE              = 3;
/**
 * \brief Remove dir key
 */
const int VISHNU_REMOVE_DIR               = 4;
/**
 * \brief Ch grp key
 */
const int VISHNU_CH_GRP                   = 5;
/**
 * \brief Ch mod key
 */
const int VISHNU_CH_MOD                   = 6;
/**
 * \brief Head of file key
 */
const int VISHNU_HEAD_OF_FILE             = 7;
/**
 * \brief Tail of file key
 */
const int VISHNU_TAIL_OF_FILE             = 8;
/**
 * \brief Content of file key
 */
const int VISHNU_CONTENT_OF_FILE          = 9;
/**
 * \brief List dir key
 */
const int VISHNU_LIST_DIR                 = 10;
/**
 * \brief Copy file key
 */
const int VISHNU_COPY_FILE                = 11;
/**
 * \brief Copy async file key
 */
const int VISHNU_COPY_ASYNC_FILE          = 12;
/**
 * \brief Move file key
 */
const int VISHNU_MOVE_FILE                = 13;
/**
 * \brief Move async file key
 */
const int VISHNU_MOVE_ASYNC_FILE          = 14;
/**
 * \brief Stop file transfer key
 */
const int VISHNU_STOP_FILE_TRANSFER       = 15;
/**
 * \brief List file transfers key
 */
const int VISHNU_LIST_FILE_TRANSFERS      = 16;
/**
 * \brief Get file info key
 */
const int VISHNU_GET_FILE_INFO            = 17;
/**
 * \brief Change owner key
 */
const int VISHNU_CH_OWN                   = 18;


/**
 * \class FMSMapper
 * \brief Mapper implementation for the FMS module
 */
class FMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \fn FMSMapper(MapperRegistry* reg, string name)
   * \param reg Registry to register
   * \param name Name to use to register
   */
  FMSMapper(MapperRegistry* reg, string name);
  /**
   * \brief Default constructor
   * \fn FMSMapper()
   */
  FMSMapper();
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
   * \brief To decode the create file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeCreateFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the create dir call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeCreateDir(vector<int> separator, const string& msg);

  /**
   * \brief To decode the remove file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeRemoveFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the remove dir call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeRemoveDir(vector<int> separator, const string& msg);

  /**
   * \brief To decode the change group call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeChGrp(vector<int> separator, const string& msg);

  /**
   * \brief To decode the change owner call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeChOwn(vector<int> separator, const string& msg);

  /**
   * \brief To decode the change mode call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeChMod(vector<int> separator, const string& msg);

  /**
   * \brief To decode the head of file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeHeadOfFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the tail of file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeTailOfFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the content of file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeContentOfFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the list dir call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeListDir(vector<int> separator, const string& msg);

  /**
   * \brief To decode the copy file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeCopyFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the copy async file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeCopyAsyncFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the move file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeMoveFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the move async file call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeMoveAsyncFile(vector<int> separator, const string& msg);

  /**
   * \brief To decode the stop file transfer call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeStopFileTransfer(vector<int> separator, const string& msg);

  /**
   * \brief To decode the list file transfers call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeListFileTransfers(vector<int> separator, const string& msg);

  /**
   * \brief To decode the get file info call sequence of the string returned by finalize
   * \param separator A vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  string
    decodeGetFileInfo(vector<int> separator, const string& msg);

private:
};


#endif
