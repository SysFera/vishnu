/**
 * \file zhelpers.hpp
 * \brief This file contains a set of helpers for zmq
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _ZHELPERS_HPP_
#define _ZHELPERS_HPP_

#include <zmq.hpp>
#include "utils.hpp"


namespace {
  const int DEFAULT_TIMEOUT = 120; // seconds
}

/**
 * \class Socket
 * \brief wraps zmq::socket_t to simplify its use
 * with std::string
 */
class Socket : public zmq::socket_t, public boost::noncopyable {
public:
  /**
   * \brief Constructor
   * \param ctx the zmq context
   * \param type The type of the socket
   */
  Socket(zmq::context_t& ctx, int type);

  /**
   * \brief set linger period for socket shutdown
   * \param linger linger time (default: -1 means infinite)
   * \returns true if it succeeded
   */
  bool
  setLinger(int linger = -1);

  /**
   * \brief wraps zmq::socket_t connect
   * \param addr connection uri
   * \return void
   * \throw error_t if it fails
   */
  void
  connect(const std::string& addr);
  /**
   * \brief wraps zmq::socket_t connect
   * \param addr connection uri
   * \return void
   * \throw error_t if it fails
   */
  void
  connect(const char* addr);

  /**
   * \brief send data
   * \param data string to be sent
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const std::string& data, int flags = 0);

  /**
   * \brief send data
   * \param data buffer to be sent
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const char* data, int flags = 0);

  /**
   * \brief get response for server
   * \param flags zmq flags
   * \return message
   */
  std::string
  get(int flags = 0);

private:
  /**
   * \brief internal method that sends message
   * \param data buffer to be sent
   * \param len size of the buffer
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const char* data, size_t len, int flags = 0);
};


/**
 * \class LazyPirateClient
 * \brief implements the Lazy Pirate pattern, argh matey !
 */
class LazyPirateClient {
public:
  /**
   * \brief Constructor
   * \param ctx the zmq context
   * \param addr the address to bind
   * \param timeout the timeout before retrying to send the message
   */
  LazyPirateClient(zmq::context_t& ctx,
                   const std::string& addr,
                   const int& timeout = DEFAULT_TIMEOUT,
                   int verbosity = 1);

  /**
   * \brief most of the pattern is implemented here
   * \param data message to be sent
   * \param retries number of retries
   * \return true if it succeeded
   */
  bool
  send(const std::string& data, int retries = 3);

  /**
   * \brief Get the message received
   */
  std::string
  recv() const;


  /**
   * \brief Reset the connection
   */
  void
  reset();

private:

  /**
   * \brief The address
   */
  std::string addr_;
  /**
   * \brief The buffer storing the messages
   */
  std::string buff_;
  /**
   * \brief The context
   */
  zmq::context_t& ctx_;
  /**
   * \brief The socket
   */
  boost::scoped_ptr<Socket> sock_;
  /**
   * \brief The timeout
   */
  long timeout_;
  /**
   * \brief The verbosity of the communication
   */
  int _verbosity;
};

#endif /* _ZHELPERS_HPP_ */
