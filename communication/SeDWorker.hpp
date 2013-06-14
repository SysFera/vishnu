/**
 * \file SeDWorker.hpp
 * \brief This file contains the definition of the workers for the servers
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _SEDWORKER_HPP_
#define _SEDWORKER_HPP_

#include "Worker.hpp"
#include "SeD.hpp"

/**
 * \class SeDWorker
 * \brief Worker
 */
class SeDWorker : public Worker {
public:
  /**
   * \brief Constructor
   * \param ctx the zmq context
   * \param uriInproc the uri for the inproc
   * \param id the id of the worker
   * \param server the corresponding server
   **/
  explicit SeDWorker(boost::shared_ptr<zmq::context_t> ctx,
                     const std::string& uriInproc, int id,
                     boost::shared_ptr<SeD> server,
                     bool usessl,
                     const std::string& certCaFile)
    : Worker(ctx, uriInproc, id), server_(server), useSsl(usessl), cafile(certCaFile) {
  }

private:
  /**
   * \brief  path to the CA file
  */
  bool useSsl;

  /**
   * \brief  path to the CA file
  */
  std::string cafile;

  /**
   * \brief Call the function
   * \param data the serialized data containing the funcion and its parameters
   * \return the serialized data (out data are updated)
   */
  std::string
  doCall(std::string& data) {
    boost::shared_ptr<diet_profile_t> profile(my_deserialize(data));
    int ret = server_->call(profile.get());
    if (ret != 0) {
      std::cerr << boost::format("[ERROR] call(profile.get()) returned %1%."
                                 " See the previous error for details.\n")%ret;
    }
    return my_serialize(profile.get());
  }

private:
  /**
   * \brief The server
   */
  boost::shared_ptr<SeD> server_; /**< Server implementation */
};



#endif /* _SEDWORKER_HPP_ */
