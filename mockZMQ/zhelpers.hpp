#ifndef _ZHELPERS_HPP_
#define _ZHELPERS_HPP_

#include "zmq.hpp"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <string.h>


const int DEFAULT_TIMEOUT = 2; // seconds

class Socket : public zmq::socket_t, public boost::noncopyable {
public:
  Socket(zmq::context_t& ctx, int type) : zmq::socket_t(ctx, type) {}

  bool
  setLinger(int linger = -1) {
    try {
      setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
      return true;
    } catch (const zmq::error_t& e) {
      return false;
    }
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
      throw zmq::error_t();
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
	LazyPirateClient(zmq::context_t& ctx, const std::string& addr, const int timeout = DEFAULT_TIMEOUT)
          : addr_(addr), ctx_(ctx), timeout_(timeout * 1000000) {
		reset();
	}

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
						std::cerr << "W: no response from "+addr_+", retrying ...\n";
						reset();
						sock_->send(data);
					}
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
	long timeout_;
};

// Extern code

/*  =========================================================================
    zhelpers.h - ZeroMQ helpers for example applications

    Copyright (c) 1991-2010 iMatix Corporation and contributors

    This is free software; you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
    =========================================================================
*/

// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>


//  Convert string to 0MQ string and send to socket
static bool
s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy(message.data(), string.data(), string.size());

    bool rc = socket.send(message);
    return (rc);
}

//  Receive 0MQ string from socket and convert into string
static std::string
s_recv (zmq::socket_t & socket) {

    zmq::message_t message;
    socket.recv(&message);

    return std::string(static_cast<char*>(message.data()), message.size());
}



#endif /* _ZHELPERS_HPP_ */
