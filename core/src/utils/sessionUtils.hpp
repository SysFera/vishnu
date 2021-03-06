/**
 * \file sessionUtils.hpp
 * \brief This file contains a declaration of class to handle a storage of
 * session key
 */
#ifndef SESSIONUTILS_HPP
#define SESSIONUTILS_HPP

#include <fstream>
#include <list>
#include <string>

// object to serialize
#include "UMS_Data.hpp"

namespace boost { namespace serialization { class access; } }


class VishnuException;

/**
 * \class SessionEntry
 * \brief A helper class to handle session authentication
 */
class SessionEntry{
public:

  /**
     * \brief A default constructor
     *\param session is an object containing the encrypted identifier of the session generated by VISHNU
     */

  explicit
  SessionEntry(const UMS_Data::Session& session);


  /**
     * \brief A default constructor
     * \param closePolicy_: the close session type with default value equal to 0
     */
  explicit
  SessionEntry(int closePolicy_=0);

  /**
     * \brief Another constructor taking parameters
     * \param sessionKey_: the key of the session
     * \param closePolicy_: the session close type with 0 as default value
     */
  explicit
  SessionEntry(const std::string& sessionKey_, int closePolicy_=0);

  /**
     * \brief To get the key of the session
     * \return The key of the session
     */
  std::string
  getSessionKey()const;

  /**
     * \brief To get the session close type
     * \return The session close type
     */
  int
  getClosePolicy()const;

  /**
     * \brief To get the session id
     * \return The session id
     */
  std::string
  getSessionId()const;

  /**
   * @brief Get the session object
   * @return
   */
  UMS_Data::Session&
  getSession(void) {return msession;}


  /**
       * \brief A helper to display a session parameters
       * \param os : A stream where the session parameters will be printed
       * \param session : The session to print
       * \return The stream where the session parameters have been printed
       */
  friend
  std::ostream&
  operator <<(std::ostream& os, const SessionEntry& session);

private:

  /**
     * \brief A class to give access to sessionEntry class
     */
  friend class boost::serialization::access;

  /**
     * \brief The serializer function
     * \param ar: An archive type which can be text type or xml type
     * \param version: The version of serialized class
     */

  template<class Archive>
  void
  serialize(Archive& ar, const unsigned int version);

  /**
     * \brief The session object
     */
  UMS_Data::Session msession;
};

/**
 * \brief A container for session
 */
typedef std::list<SessionEntry> SessionContainer;

/**
 * \brief To record  sessions in a file
 * \param allSessions: A set of sessions which will be stored in the file
 * \param file       : the path of the file
 * \exception        : raises a Boost archive exception
 */

template <class T>
void
saveIntoFile(SessionContainer& allSessions, const char* file);


/**
 * \brief  To load all sessions stored in a file
 * \param allSessions: The sessions stored in the file
 * \param file       : The file storing the sessions
 * \exception        : raises a Boost archive exception
 */

template <class T>
void
getFromFile(SessionContainer& allSessions, const char* file);

/**
 * \brief helper function which which wrappes getFromFile function
 * and ensures that all error exception is catched .
 * \param terminal: the file corresponding to the terminal which launched the
 * serialization process
 * \return all sessions stored into the file or an empty container
 */

SessionContainer
getAllSessions(const std::string& terminal);

/**
 * \brief To store a the current session running in a terminal in the file
 * \param session : the current session
 * \param terminal: The terminal in which the session is running
 */
void
storeLastSession(const SessionEntry& session ,const std::string& terminal);

/**
 * \brief Another function to store last session open
 * \param session_: The  session which will be stored
 * \param ppid: The identifier of the terminal as process
 */


void
storeLastSession(const UMS_Data::Session& session_,int ppid);

/**
 * To get the last open session corrseponding to the last session stored in the
 * file
 * \param terminal : The terminal in which the session was running
 * \return The last stored session
 */

SessionEntry
getLastSession(const std::string& terminal);

/**
 * \brief To remove the last entry of session from the file
 * \param terminal : The terminal in which the session was running
 */
void
removeLastSession(const std::string & terminal);

/**
 * \brief To get the path of the file in which the sessions open from a terminal
 * had been stored
 * \param tpid : the identifier of the terminal as process
 * \return the path of the file in which the sessions open from the terminal had
 * been stored
 */

std::string
getSessionLocation(int tpid);

/**
 * \brief Another function to store last session open
 * \param sessionKey: The key of the session which will be stored
 * \param closePolicy: The session close type
 * \param ppid: The identifier of the terminal as process
 */

void
storeLastSession(const std::string& sessionKey,int closePolicy,int ppid);

/**
 * \brief To get the last session key
 * \param ppid: The identifier of the terminal as process in which the session
 * had been open
 * \return The key of the last session open from the terminal
 */

std::string
getLastSessionKey(int ppid);

/**
 * \brief To get the last session identifier
 * \param ppid: The identifier of the terminal as process in which the session
 * had been open
 * \return The identifier of the last session open from the terminal
 */

std::string
getLastSessionId(int ppid);


/**
 * \brief To remove the bad session keys stored in the session file
 * \param ppid : The process identifier of the terminal in which the session had
 * been open.
 */
void
removeBadSessionKeyFromFile(int ppid);


/**
 * \brief A helper function which check if an given exception represents a bad
 * sessionKey
 *\param e: a given VishnuException
 *\return   : true if it's a bad session key or false otherwhise
 */
bool
checkBadSessionKeyError(const VishnuException& e);

#endif
