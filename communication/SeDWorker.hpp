#ifndef _SEDWORKER_HPP_
#define _SEDWORKER_HPP_

#include "Worker.hpp"
#include "SeD.hpp"


class SeDWorker : public Worker {
public:
  explicit SeDWorker(boost::shared_ptr<zmq::context_t> ctx,
                     const std::string& uriInproc, int id,
                     boost::shared_ptr<SeD> server)
    : Worker(ctx, uriInproc, id), server_(server) {
  }

private:
  std::string
  doCall(std::string& data) {
    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
    server_->call(profile.get());
    // Send reply back to client
    return my_serialize(profile.get());
  }

private:
  boost::shared_ptr<SeD> server_; /**< Server implementation */
};



#endif /* _SEDWORKER_HPP_ */
