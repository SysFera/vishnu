#ifndef _ZHELPERS_HPP_
#define _ZHELPERS_HPP_

#include <zmq.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>



class Socket : public zmq::socket_t, public boost::noncopyable {
public:
  Socket(zmq::context_t& ctx, int type) : zmq::socket_t(ctx, type) {}

  bool
  setLinger(int linger = -1) {
    setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
  }

  bool
  send(const std::string& data, int flags = 0) {
    return send(data.c_str(), data.length(), flags);
  }

  bool
  send(const char* data, int flags = 0) {
    return send(data, strlen(data), flags);
  }

  std::string
  get(int flags = 0) {
    zmq::message_t message;
    if (!recv(&message, flags)) {
      throw error_t();
    }

    const char *dat = static_cast<const char*>(message.data());
    return std::string(dat, dat + message.size());
  }

private:
  bool
  send(const char* data, size_t len, int flags = 0) {
    zmq::message_t msg(len);
    memcpy(msg.data(), data, len);

    return socket_t::send(msg, flags);
  }
};

class LazyPirateClient {
public:
  LazyPirateClient(zmq::context_t& ctx, std::string& addr)
    : ctx_(ctx), addr_(addr) {
    reset();
  }

  bool
  send(std::string& data, int retries = 3, int timeout = 2500) {
    while (retries) {
      sock_->send(data);
      bool expect_reply(true);

      while (expect_reply) {
        zmq::pollitem_t items[] = { {*sock_, 0, ZMQ_POLLIN, 0} };
        zmq::poll(&items[0], 1, timeout * 1000);

        if (items[0].revents & ZMQ_POLLIN) {
          buff_ = sock_->get();
          if (buff_.length()) {
            return true;
          } else {
            std::cerr << "E: received weird reply from server\n";
          }
        } else if (0 == (--retries)) {
          std::cerr << "E: server seems offline, abandonning\n";
          expect_reply = false;
          break;
        } else {
          std::cerr << "W: no response from server, retrying ...\n";
          reset();
          sock_->send(data);
        }
      }
    }

    return false;
  }


  std::string
  recv() {
    return buff_;
  }



private:
  void
  reset() {
    sock_.reset(new Socket(ctx_, ZMQ_REQ));
    sock_->connect(addr_.c_str());
    sock_->setLinger(0);
  }

  std::string addr_;
  std::string buff_;
  zmq::context_t& ctx_;
  boost::scoped_ptr<Socket> sock_;
};





#endif /* _ZHELPERS_HPP_ */
