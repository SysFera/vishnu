/**
 * \file zhelpers.hpp
 * \brief This file contains a set of helpers for zmq
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _ZHELPERS_HPP_
#define _ZHELPERS_HPP_

#include <iostream>
#include <cstring>
#include <cerrno>

#include <zmq.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "utils.hpp"
#include "sslhelpers.hpp"

/**
 * \brief The default timeout value used to comunicate
 */
const int DEFAULT_TIMEOUT = 120; // seconds

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
  Socket(zmq::context_t& ctx, int type, SslCrypto* ciph) : zmq::socket_t(ctx, type), cipher(ciph) {}

  /**
   * \brief set linger period for socket shutdown
   * \param linger linger time (default: -1 means infinite)
   * \returns true if it succeeded
   */
  bool
  setLinger(int linger = -1) {
    try {
      setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
      return true;
    } catch (const zmq::error_t& e) {
      return false;
    }
  }

  /**
   * \brief wraps zmq::socket_t connect
   * \param addr connection uri
   * \return void
   * \throw error_t if it fails
   */
  void
  connect(const std::string& addr) {
    connect(addr.c_str());
  }

  /**
   * \brief wraps zmq::socket_t connect
   * \param addr connection uri
   * \return void
   * \throw error_t if it fails
   */
  void
  connect(const char* addr) {
    /* we explicitely call base class method to avoid
       looping method calls */
    socket_t::connect(addr);
  }

  /**
   * \brief send data
   * \param data string to be sent
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const std::string& data, int flags = 0) {
    return send(data.c_str(), data.length()+1, flags);
  }

  /**
   * \brief send data
   * \param data buffer to be sent
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const char* data, int flags = 0) {
    return send(data, strlen(data)+1, flags);
  }

  /**
   * \brief get response for server
   * \param flags zmq flags
   * \return message
   */
  std::string
  get(int flags = 0) {
    zmq::message_t message;
    bool rv = false;

    do {
      try {
        rv = recv(&message, flags);
        break;
      } catch (const zmq::error_t& e) {
        if (EINTR == e.num()) {
          continue;
        } else {
          throw e;
        }
      }
    } while(true);

    if (!rv) {
      throw zmq::error_t();
    }

    char* decData;
    int decDataLength = message.size();
    if (cipher != NULL) {
      decDataLength = cipher->aesDecrypt(reinterpret_cast<unsigned char*>(message.data()), decDataLength, &decData);
    } else {
      decData = static_cast<char*>(message.data());
    }
    const char* pos = decData + decDataLength;
    return std::string(static_cast<const char*>(decData), pos);
  }

private:
  /**
      *\brief Pointer to the encryption object
    */
  SslCrypto* cipher;

  /**
   * \brief internal method that sends message
   * \param data buffer to be sent
   * \param len size of the buffer
   * \param flags zmq flags
   * \return true if it succeeded
   */
  bool
  send(const char* data, size_t len, int flags = 0) {

    unsigned char* encData;
    int encDataLength = len;
    if (cipher != NULL) {
      encDataLength = cipher->aesEncrypt(data, len, &encData);
    } else {
      char* tmp = const_cast<char*>(data);
      encData = reinterpret_cast<unsigned char*>(tmp);
    }

    zmq::message_t msg(encDataLength);
    memcpy(msg.data(), encData, encDataLength);

    return socket_t::send(msg, flags);
  }
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
                   SslCrypto* ciph,
                   const int timeout = DEFAULT_TIMEOUT)
    : addr_(addr), ctx_(ctx), timeout_(timeout * 1000000), cipher(ciph) {
    reset();
  }

  /**
   * \brief most of the pattern is implemented here
   * \param data message to be sent
   * \param retries number of retries
   * \return true if it succeeded
   */
  bool
  send(const std::string& data, int retries = 3) {
    while (retries) {
      sock_->send(data);
      bool expect_reply(true);

      while (expect_reply) {
        zmq::pollitem_t items[] = { {*sock_, 0, ZMQ_POLLIN, 0} };
        zmq::poll(&items[0], 1, timeout_);

        if (items[0].revents & ZMQ_POLLIN) {
          buff_ = sock_->get();
          if (buff_.length()) {
            return true;
          } else {
            std::cerr << "E: received weird reply from server\n";
          }
        } else {
          if (--retries == 0) {
            std::cerr << "E: server seems offline, abandonning\n";
            expect_reply = false;
            break;
          } else {
            std::cerr << boost::format("W: no response from %1%, retrying ...\n") % addr_;
            reset();
            sock_->send(data);
          }
        }
      }
    }
    return false;
  }

  /**
   * \brief Get the message received
   */
  std::string
  recv() const {
    return buff_;
  }


private:
  /**
   * \brief Reset the connection
   */
  void
  reset() {
    sock_.reset(new Socket(ctx_, ZMQ_REQ, cipher));
    sock_->connect(addr_);
    sock_->setLinger(0);
  }

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
      *\brief Pointer to the encryption object
    */
  SslCrypto* cipher;
};



#endif /* _ZHELPERS_HPP_ */
