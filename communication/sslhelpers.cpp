#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

#include "sslhelpers.hpp"
#include "SystemException.hpp"
#include "utilVishnu.hpp"

void TlsServer::run()
{
  /* Initialize the OpenSSL Library */
  SSL_library_init();
  ERR_load_crypto_strings();
  ERR_load_SSL_strings();
  OpenSSL_add_all_algorithms();

  // Creating a SSL context
  SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
  if(!ctx) {
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
  if(ssl == NULL) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Enable the handling of retry from clients */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Create a buffer and add it to chain */
  BIO* bio4Buffer = BIO_new(BIO_f_buffer());
  clientBioHandler = BIO_push(bio4Buffer, clientBioHandler);

  char* portStr = const_cast<char*>(vishnu::convertToString(listeningPort).c_str());
  BIO* acceptBio = BIO_new_accept(portStr);
  if (acceptBio == NULL) {
    errorMsg = (boost::format("Failed initilizing TLS listener.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  BIO_set_accept_bios(acceptBio, clientBioHandler);
  if(BIO_do_accept(acceptBio) <= 0) {
    errorMsg = (boost::format("Failed setting up accepting BIO.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Initialize a ZMQ LPC client */
  zmq::context_t zctx(1);
  LazyPirateClient zlpc(zctx, internalServiceUri, getTimeout());

  /* Now wait for incoming connections */
  while (1) {
    if(BIO_do_accept(acceptBio) <= 0) { /* Wait for new connection */
      errorMsg = (boost::format("Failed connecting a client.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
    }

    clientBioHandler = BIO_pop(acceptBio);

    if (BIO_do_handshake(clientBioHandler) <= 0) {
      std::cerr << boost::format("SSL handshake.\n%1%\n"
                                 )%ERR_error_string(ERR_get_error(), NULL);
      continue;
    }

    recvMsg();

    if (!data.empty()) {
      std::string reply;
      zlpc.send(data); /* Forward the message to the service handler */

      reply = zlpc.recv();
      reply.append("\n\n"); /* necessary for the communication protocol between server and clients */
      int len = BIO_write(clientBioHandler, reply.c_str(), reply.size());
      BIO_flush(clientBioHandler);
      if (len <= 0 && !reply.empty()) {
        std::cout << boost::format("[WARNING] %1%/%2% bytes written\n")%len%reply.size();
      }
    } else {
      std::cerr << boost::format("[ERROR] Empty message reveived.\n%1%\n"
                                 )%ERR_error_string(ERR_get_error(), NULL);
    }
    BIO_free_all(clientBioHandler);
  }
}


void TlsServer::recvMsg()
{
  char msgBuf[MSG_CHUNK_SIZE];
  data.clear();
  int len;
  while ((len = BIO_gets(clientBioHandler, msgBuf, MSG_CHUNK_SIZE))> 0) {
    if((msgBuf[0] == '\r') || (msgBuf[0] == '\n')) break;
    data.append(std::string(msgBuf, len));
  }
}


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
  if(!sslctx) {
    errorMsg = (boost::format("Failed getting SSL_CTX.\n%1%")
                %ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Load trust store */
  if(! SSL_CTX_load_verify_locations(sslctx, cafile.c_str(), NULL)) {
    errorMsg = (boost::format("Failed loading trust store.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Setup the SSL BIO as client */
  SSL* ssl;
  sslBio = BIO_new_ssl_connect(sslctx);
  BIO_get_ssl(sslBio, &ssl);
  if(!ssl) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Enable retry */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Now connect to server */
  std::string addr = (boost::format("%1%:%2%")%serverAddr%serverPort).str();
  BIO_set_conn_hostname(sslBio, const_cast<char*>(addr.c_str()));
  if(BIO_do_connect(sslBio) <= 0) {
    errorMsg = (boost::format("Failed connecting to server.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  if(SSL_get_verify_result(ssl) != X509_V_OK) {
    errorMsg = (boost::format("Failed verifying the server certificate.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  if(BIO_do_handshake(sslBio) <= 0) {
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

  int len;
  len = BIO_puts(sslBio, reqData.c_str());
  if (len <= 0 && !reqData.empty()) {
    std::cout << boost::format("[INFO] %1%/%2% bytes sent\n")%len%reqData.size();
  }

  return 0;
}

/**
 * @brief recv
 * @return
 */
std::string TlsClient::recv(void)
{
  recvMsg();
  return data;
}

void TlsClient::recvMsg()
{
  char msgBuf[MSG_CHUNK_SIZE];
  data.clear();
  int len;
  while ((len = BIO_read(sslBio, msgBuf, MSG_CHUNK_SIZE))> 0) {
    if((msgBuf[0] == '\r') || (msgBuf[0] == '\n')) break;
    data.append(std::string(msgBuf, len));
  }
}
