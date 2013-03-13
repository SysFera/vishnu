/**
 * \file TMSMapper.hpp
 * \brief Mapper for the TMS module
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#ifndef __TMSMAPPER__HH__
#define __TMSMAPPER__HH__

#include <string>
#include <vector>

#include "Mapper.hpp"

class MapperRegistry;

/**
 * \brief Submit job key
 */
const int VISHNU_SUBMITJOB               = 1;
/**
 * \brief Get job info key
 */
const int VISHNU_GETJOBINFO              = 2;
/**
 * \brief Get job progress key
 */
const int VISHNU_GETJOBPROG              = 3;
/**
 * \brief List queues key
 */
const int VISHNU_LISTQUEUES              = 4;
/**
 * \brief List jobs key
 */
const int VISHNU_LISTJOBS                = 5;
/**
 * \brief Get job output key
 */
const int VISHNU_GETJOBOUTPUT            = 6;
/**
 * \brief Get all job output key
 */
const int VISHNU_GETCOMPLETEDJOB         = 7;
/**
 * \brief Cancel key
 */
const int VISHNU_CANCEL                  = 8;
/**
 * \brief Add a work
 */
const int VISHNU_ADD_WORK                  = 9;

/**
 * \class TMSMapper
 * \brief Mapper implementation for the TMS module
 */
class TMSMapper : Mapper{
public :
  /**
   * \brief Constructor
   * \fn TMSMapper(MapperRegistry* reg, std::string name)
   * \param reg Registry to register
   * \param name Name to use to register
   */
  TMSMapper(MapperRegistry* reg, std::string name);
  /**
   * \brief Default constructor
   * \fn TMSMapper()
   */
  TMSMapper();
  /**
   * \brief Destructor
   * \fn ~TMSMapper()
   */
  ~TMSMapper();
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
   * \fn virtual int getCommand(const int& key,std::string& command)
   * \param key The key to use
   * \param command OUT, the command corresponding to the key
   * \return 0 on success, an error code otherwise
   */
  virtual int
  getCommand(const int& key,std::string& command);

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
   * \param code If 0, the code is created, otherwize it is the key in the map to add the std::string
   * \return if param code = 0, the assigned code to add other element to the same item, otherwize return 0
   */
  virtual int
  code(const std::string& cmd, unsigned int code = 0);

  /**
   * \brief To get, from a coded std::string, the cli like command that made it
   * \fn virtual std::string decode (const std::string& msg);
   * \param msg The coded std::string
   * \return The cli like command
   */
  virtual std::string
  decode (const std::string& msg);

protected:
  /**
   * \brief To decode the submit call sequence of the std::string returned by finalize
   * \fn std::string decodeSubmit(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeSubmit(std::vector<unsigned int> separator, const std::string& msg);
  /**
   * \brief To decode the add work call sequence of the std::string returned by finalize
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeAddWork(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get progression call sequence of the std::string returned by finalize
   * \fn std::string decodeProg(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeProg(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get queue call sequence of the std::string returned by finalize
   * \fn std::string decodeQueue(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeQueue(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get list job call sequence of the std::string returned by finalize
   * \fn std::string decodeListJob(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeListJob(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get output call sequence of the std::string returned by finalize
   * \fn std::string decodeOutput(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeOutput(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get all output call sequence of the std::string returned by finalize
   * \fn std::string decodeCompletedJob(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeCompletedJob(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the cancel call sequence of the std::string returned by finalize
   * \fn std::string decodeCancel(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeCancel(std::vector<unsigned int> separator, const std::string& msg);

  /**
   * \brief To decode the get job info call sequence of the std::string returned by finalize
   * \fn std::string decodeJobInfo(std::vector<unsigned int> separator, const std::string& msg)
   * \param separator A std::vector containing the position of the separator in the message msg
   * \param msg The message to decode
   * \return The cli like close command
   */
  virtual std::string
  decodeJobInfo(std::vector<unsigned int> separator, const std::string& msg);
private:
};


#endif
