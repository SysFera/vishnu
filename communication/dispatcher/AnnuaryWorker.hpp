#ifndef _ANNUARYWORKERS_HPP_
#define _ANNUARYWORKERS_HPP_

#include "Worker.hpp"
#include "DIET_client.h"
#include "UserException.hpp"
#include "Annuary.hpp"

class AnnuaryWorker : public Worker {
public:
  explicit AnnuaryWorker(boost::shared_ptr<zmq::context_t> ctx,
                         const std::string& uriInproc, int id,
                         boost::shared_ptr<Annuary> ann)
    : Worker(ctx, uriInproc, id), mann_(ann) {}

protected:
  boost::shared_ptr<Annuary> mann_;
};




class ServiceWorker : public AnnuaryWorker {
public:
  explicit ServiceWorker(boost::shared_ptr<zmq::context_t> ctx,
                         const std::string& uriInproc, int id,
                         boost::shared_ptr<Annuary> ann)
    : AnnuaryWorker(ctx, uriInproc, id, ann) {}

private:
  std::string
  doCall(std::string& data) {
    using boost::format;
    using boost::str;

    boost::shared_ptr<diet_profile_t> profile = my_deserialize(data);
    std::string servname = profile->name;
    std::vector<boost::shared_ptr<Server> > serv = mann_->get(servname);
    std::string uriServer = elect(serv);

    if (!uriServer.empty()) {
      diet_call_gen(profile.get(), uriServer);
      return my_serialize(profile.get());
    } else {
      return str(format("error %1%: the service %2% is not available")
                 % ERRCODE_INVALID_PARAM
                 % servname);
    }
  }

  std::string
  elect(const std::vector<boost::shared_ptr<Server> >& serv){
    if (serv.empty()) {
      return "";
    }
    return serv.at(0)->getURI();
  }

};

//FIXME: clear unused parameters
class SubscriptionWorker : public AnnuaryWorker {
public:
  explicit SubscriptionWorker(boost::shared_ptr<zmq::context_t> ctx,
                              const std::string& uriInproc, int id,
                              boost::shared_ptr<Annuary> ann)
    : AnnuaryWorker(ctx, uriInproc, id, ann) {}

private:
  std::string
  doCall(std::string& data) {
    // std::string data(static_cast<char*>(message.data()), message.size());
    int mode = boost::lexical_cast<int>(data.substr(0,1));

    // Deserialize
    boost::shared_ptr<Server> server = Server::fromString(data.substr(1));

    if (mode == 1) {
      // adding a new server
      mann_->add(server->getName(), server->getURI(),
                server->getServices());
    } else if (mode == 0) {
      mann_->remove(server->getName(), server->getURI());
    }

    return "OK";
  }

};

#endif /* _ANNUARYWORKERS_HPP_ */
