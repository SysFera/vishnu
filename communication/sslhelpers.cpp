/**
 * \file sslhelpers.hpp
 * \brief This file contains the definition of the base classes (client/server) for the TLS
 * \author Rodrigue Chakode (rodrigue.chakode@sysfera.com)
 * \date May 2013
 */

#include <boost/algorithm/string/predicate.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

#include "sslhelpers.hpp"
#include "SystemException.hpp"
#include "utilVishnu.hpp"


/**
 * @brief TlsServer::run
 */
void
TlsServer::run()
{
  /* Initialize the OpenSSL Library */
  SSL_library_init();
  ERR_load_crypto_strings();
  ERR_load_SSL_strings();
  OpenSSL_add_all_algorithms();

  // Creating a SSL context
  SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    errorMsg = (boost::format("Failed getting SSL_CTX.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  // Set the private key and the certificate
  if (!SSL_CTX_use_certificate_file(ctx, certificate.c_str(), SSL_FILETYPE_PEM)
      || !SSL_CTX_use_PrivateKey_file(ctx, privateKey.c_str(), SSL_FILETYPE_PEM)
      || !SSL_CTX_check_private_key(ctx)) {
    errorMsg = (boost::format("Failed setting SSL_CTX.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Setup the SSL BIO as server */
  SSL* ssl = NULL;
  clientBioHandler = BIO_new_ssl(ctx,0);
  BIO_get_ssl(clientBioHandler, &ssl);
  if (ssl == NULL) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Enable the handling of retry from clients */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Create a buffer and add it to chain */
  BIO* bio4Buffer = BIO_new(BIO_f_buffer());
  clientBioHandler = BIO_push(bio4Buffer, clientBioHandler);

  std::string addr = (boost::format("0.0.0.0:%1%")%listeningPort).str();
  BIO* acceptBio = BIO_new_accept(const_cast<char*>(addr.c_str()));
  if (acceptBio == NULL) {
    errorMsg = (boost::format("Failed binding for client connections (%1%).\n%2%"
                              )%addr%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  } else {
    std::cout << boost::format("[INFO] TLS socket bound (%1%)\n")%addr;
  }

  BIO_set_accept_bios(acceptBio, clientBioHandler);
  if (BIO_do_accept(acceptBio) <= 0) {
    errorMsg = (boost::format("Failed starting the TLS listener (%1%).\n%2%"
                              )%addr%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Initialize a ZMQ LPC client */
  zmq::context_t zctx(1);
  LazyPirateClient zlpc(zctx, internalServiceUri, getTimeout());

  /* Now wait for incoming connections */
  while (1) {
    if (BIO_do_accept(acceptBio) <= 0) { /* Wait for new connection */
      errorMsg = (boost::format("Failed connecting a client.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
    }

    clientBioHandler = BIO_pop(acceptBio);

    if (BIO_do_handshake(clientBioHandler) <= 0) {
      std::cerr << boost::format("Failed making SSL handshake.\n%1%\n"
                                 )%ERR_error_string(ERR_get_error(), NULL);
      continue;
    }

    recvMsg();

    if (!data.empty()) {
      zlpc.send(data); /* Forward the message to the service handler */
      std::vector<std::string> msgs;
      msgs.push_back(zlpc.recv());
      sendMsgs(msgs);
    } else {
      std::cerr << boost::format("[WARNING] Empty message reveived.\n%1%\n"
                                 )%ERR_error_string(ERR_get_error(), NULL);
    }
    BIO_free_all(clientBioHandler);
  }
}

/**
 * @brief TlsServer::recvMsg
 */
void
TlsServer::recvMsg()
{
  char msgBuf[MSG_CHUNK_SIZE];
  data.clear();
  int len;
  while ((len = BIO_gets(clientBioHandler, msgBuf, MSG_CHUNK_SIZE))> 0 &&
         ! boost::algorithm::starts_with(msgBuf, END_OF_SSL_MSG)) {

    data.append(std::string(msgBuf, len));
  }
}

/**
 * @brief TlsServer::sendMsgs
 * @param msgs
 */
void
TlsServer::sendMsgs(std::vector<std::string>& msgs)
{
  msgs.push_back(END_OF_SSL_MSG);
  for (std::vector<std::string>::iterator msg=msgs.begin(), end=msgs.end(); msg!=end;++msg) {
    msg->append("\n");
    int len = BIO_write(clientBioHandler, msg->c_str(), msg->size());
    BIO_flush(clientBioHandler);
    if (len <= 0 && !msg->empty()) {
      std::cout << boost::format("[WARNING] %1%/%2% bytes written\n")%len%msg->size();
    }
  }
}




/**
 * @brief TlsClient::send
 * @param reqData
 * @return
 */
int
TlsClient::send(const std::string& reqData)
{
  /* Initializing the library */
  SSL_library_init();
  ERR_load_crypto_strings();
  ERR_load_SSL_strings();
  OpenSSL_add_all_algorithms();


  // Creating a SSL context
  SSL_CTX* sslctx = SSL_CTX_new(SSLv23_client_method());
  if (sslctx == NULL) {
    errorMsg = (boost::format("Failed getting SSL_CTX.\n%1%")
                %ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Load trust store if set */
  if (!cafile.empty()) {
    if(! SSL_CTX_load_verify_locations(sslctx, cafile.c_str(), NULL)) {
      errorMsg = (boost::format("Failed loading trust store.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      return -1;
    }
  }

  /* Setup the SSL BIO as client */
  SSL* ssl;
  sslBio = BIO_new_ssl_connect(sslctx);
  BIO_get_ssl(sslBio, &ssl);
  if (ssl == NULL) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Enable retry */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Now connect to server */
  std::string addr = (boost::format("%1%:%2%")%serverAddr%serverPort).str();
  BIO_set_conn_hostname(sslBio, const_cast<char*>(addr.c_str()));
  if (BIO_do_connect(sslBio) <= 0) {
    errorMsg = (boost::format("Failed connecting to server (%1%).\n%2%"
                              )%addr%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  if (SSL_get_verify_result(ssl) != X509_V_OK) {
    errorMsg = (boost::format("Failed verifying the server certificate.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  if (BIO_do_handshake(sslBio) <= 0) {
    errorMsg = (boost::format("Failed establishing SSL connection.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  X509* peerCert = SSL_get_peer_certificate(ssl);;
  if (peerCert == NULL) {
    errorMsg = (boost::format("Failed getting peer certificate key.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  std::vector<std::string> msgs;
  msgs.push_back(reqData);
  sendMsgs(msgs);

  return 0;
}

/**
 * @brief recv
 * @return
 */
std::string
TlsClient::recv(void)
{
  recvMsg();
  return data;
}

/**
 * @brief TlsClient::recvMsg
 */
void
TlsClient::recvMsg() {

  char msgBuf[MSG_CHUNK_SIZE];
  data.clear();

  int len;
  while ((len = BIO_read(sslBio, msgBuf, MSG_CHUNK_SIZE))> 0 &&
         ! boost::algorithm::starts_with(msgBuf, END_OF_SSL_MSG)) {

    data.append(std::string(msgBuf, len));
  }
}


/**
 * @brief TlsClient::sendMsgs
 * @param msgs
 */
void
TlsClient::sendMsgs(std::vector<std::string>& msgs) {

  msgs.push_back(END_OF_SSL_MSG);

  for (std::vector<std::string>::iterator msg=msgs.begin(), end=msgs.end(); msg!=end;++msg) {
    msg->append("\n");
    int len = BIO_puts(sslBio, msg->c_str());
    if (len <= 0 && !msg->empty()) {
      std::cout << boost::format("[WARNING] %1%/%2% bytes sent\n")%len%msg->size();
    }
  }
}

