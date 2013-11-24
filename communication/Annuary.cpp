#include "Annuary.hpp"

#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/locks.hpp>
#include <iterator>
#include <iostream>
#include <sstream>

#include "Server.hpp"                   // for Server
#include "UMSServices.hpp"
#include "TMSServices.hpp"
#include "IMSServices.hpp"
#include "FMSServices.hpp"

Annuary::Annuary(const std::vector<boost::shared_ptr<Server> >& serv)
  : mservers(serv) {}


// anonymous namespace
namespace {
  class is_server : std::unary_function<Server, bool> {
  public:
    is_server(const std::string& name, const std::string& uri) :
      name_(name), uri_(uri) {}

    bool operator() (boost::shared_ptr<Server> serv) {
      return ((serv->getName() == name_) &&
              (serv->getURI() == uri_));
    }

  private:
    std::string name_;
    std::string uri_;
  };
}

int
Annuary::add(const std::string& name, const std::string& uri,
             const std::vector<std::string>& services) {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  is_server helper(name, uri);
  std::vector<boost::shared_ptr<Server> >::iterator it =
    std::find_if(mservers.begin(), mservers.end(), helper);
  if (it == mservers.end()) {
    mservers.push_back(boost::make_shared<Server>(name, services, uri));
  }
  return 0;
}

int
Annuary::remove(const std::string& name, const std::string& uri) {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  is_server helper(name, uri);
  mservers.erase(std::remove_if(mservers.begin(), mservers.end(), helper),
                 mservers.end());
  return 0;
}


// Note: we're using copy elision optimization here, no useless copy
std::vector<boost::shared_ptr<Server> >
Annuary::get(const std::string& service) {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  std::vector<boost::shared_ptr<Server> > res;

  if (service.empty()) {
    res.assign(mservers.begin(), mservers.end());
  } else {
    std::remove_copy_if(mservers.begin(), mservers.end(),
                        std::back_inserter(res),
                        !boost::bind(&Server::hasService, _1, service));
  }
  return res;
}


void
Annuary::print() {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  if (!mservers.empty()) {
    std::cerr << "\n==== Initial startup services ====\n";
    std::vector<boost::shared_ptr<Server> >::const_iterator it;
    for (it = mservers.begin(); it != mservers.end(); ++it) {
      std::cerr << "" << it->get()->getName() << ": " << it->get()->getURI() << "\n";
    }
    std::cerr << "==================================\n";
  } else {
    std::cerr << "\nNo initial services at startup\n";
  }
}


/**
 * \brief Initialize the annuary with the configuration of the given module
 * \param module The name of the module
 * \param cfgInfo the value of the module configuration
 */
void
Annuary::setInitConfig(const std::string& module, std::vector<std::string>& cfgInfo) {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  BOOST_FOREACH(const std::string& entry, cfgInfo) {
    std::istringstream iss(entry);
    std::string uri;
    std::string mid;
    iss >> uri;
    iss >> mid;
    std::vector<std::string> services;
    fillServices(services, module, mid);
    mservers.push_back(boost::make_shared<Server>(module, services, uri));
  }
}

void
Annuary::fillServices(std::vector< std::string> &services,
                      const std::string& name,
                      const std::string& mid) {
  boost::lock_guard<boost::recursive_mutex> lock(mmutex);
  unsigned int nb;
  std::string tmpserv;

  if (name == "UMS") {
    for (nb = 0; nb < NB_SRV_UMS; nb++) {
      services.push_back(SERVICES_UMS[nb]);
    }
  } else if (name == "TMS") {
    for (nb = 0; nb < NB_SRV_TMS; nb++) {
      tmpserv = SERVICES_TMS[nb];
      if (MACHINE_SPECIC_SERVICES_TMS[nb]) {
        tmpserv += "@" + mid;
      }
      services.push_back(tmpserv);
    }
  } else if (name == "IMS") {
    for (nb = 0; nb < NB_SRV_IMS; nb++) {
      tmpserv = SERVICES_IMS[nb];
      if (MACHINE_SPECIC_SERVICES_IMS[nb]) {
        tmpserv += "@" + mid;
      }
      services.push_back(tmpserv);
    }
  } else if (name == "FMS") {
    for (nb = 0; nb < NB_SRV_FMS; nb++) {
      services.push_back(SERVICES_FMS[nb]);
    }
  } else { // Routage
    services.push_back("routage");
  }
}
