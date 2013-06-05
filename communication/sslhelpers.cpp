#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

#include "sslhelpers.hpp"
#include "SystemException.hpp"
#include "utilVishnu.hpp"

using namespace std;

SslCrypto::SslCrypto(std::string& key, int side)
  : encSide(side),
    keypair(NULL),
    publicKey(NULL),
    rsaSymKey(NULL),
    rsaSymKeyLen(0),
    rsaIV(NULL),
    encryptLen(0)
{
  init(DEFAULT_RSA_KEYLEN, DEFAULT_AES_KEYLEN);
  setKey(key, side);
}

SslCrypto::~SslCrypto() {
  EVP_PKEY_free(keypair);
  EVP_PKEY_free(publicKey);

  EVP_CIPHER_CTX_cleanup(rsaEncryptCtx);
  EVP_CIPHER_CTX_cleanup(aesEncryptCtx);

  EVP_CIPHER_CTX_cleanup(rsaDecryptCtx);
  EVP_CIPHER_CTX_cleanup(aesDecryptCtx);

  free(rsaEncryptCtx);
  free(aesEncryptCtx);

  free(rsaDecryptCtx);
  free(aesDecryptCtx);

  free(rsaSymKey);
  free(rsaIV);

  free(aesKey);
  free(aesIV);
}

int SslCrypto::rsaEncrypt(std::string msg, unsigned char **encMsg) {
  return rsaEncrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::rsaEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg) {
  size_t encMsgLen = 0;
  size_t blockLen  = 0;
  *encMsg = (unsigned char*)malloc(EVP_PKEY_size(publicKey));
  if(encMsg == NULL) return FAILURE;
  if(!EVP_SealInit(rsaEncryptCtx, EVP_aes_128_cbc(), &rsaSymKey, &rsaSymKeyLen, rsaIV, &publicKey, 1)) {
    return FAILURE;
  }

  if(!EVP_SealUpdate(rsaEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen, (const unsigned char*)msg, (int)msgLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  if(!EVP_SealFinal(rsaEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  EVP_CIPHER_CTX_cleanup(rsaEncryptCtx);

  return (int)encMsgLen;
}

int SslCrypto::aesEncrypt(std::string msg, unsigned char **encMsg) {
  return aesEncrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::aesEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg) {
  size_t blockLen  = 0;
  size_t encMsgLen = 0;
  *encMsg = (unsigned char*)malloc(msgLen + AES_BLOCK_SIZE);
  if(encMsg == NULL) return FAILURE;

  if(!EVP_EncryptInit_ex(aesEncryptCtx,
                         EVP_aes_256_cbc(),
                         NULL,
                         aesKey,
                         aesIV)) {
    return FAILURE;
  }

  if(!EVP_EncryptUpdate(aesEncryptCtx,
                        *encMsg,
                        (int*)&blockLen,
                        (unsigned char*)msg,
                        msgLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  if(!EVP_EncryptFinal_ex(aesEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
    return FAILURE;
  }

  EVP_CIPHER_CTX_cleanup(aesEncryptCtx);
  return encMsgLen + blockLen;
}

std::string SslCrypto::rsaDecrypt(unsigned char *encMsg, size_t encMsgLen) {
  char **decMsg = NULL;
  rsaDecrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCrypto::rsaDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
  size_t decLen   = 0;
  size_t blockLen = 0;
  *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
  if(decMsg == NULL) return FAILURE;

  int success;
  if (encSide == SIDE_SERVER) {
    success = EVP_OpenInit(rsaDecryptCtx,
                           EVP_aes_128_cbc(),
                           rsaSymKey,
                           rsaSymKeyLen,
                           rsaIV,
                           keypair);
  } else {
    success = EVP_OpenInit(rsaDecryptCtx,
                           EVP_aes_128_cbc(),
                           rsaSymKey,
                           rsaSymKeyLen,
                           rsaIV,
                           publicKey);
  }

  if(!success) {
    return FAILURE;
  }

  if(!EVP_OpenUpdate(rsaDecryptCtx,
                     (unsigned char*)*decMsg + decLen,
                     (int*)&blockLen, encMsg,
                     (int)encMsgLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  if(!EVP_OpenFinal(rsaDecryptCtx, (unsigned char*)*decMsg + decLen, (int*)&blockLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  (*decMsg)[decLen] = '\0';

  EVP_CIPHER_CTX_cleanup(rsaDecryptCtx);

  return (int)decLen;
}

std::string SslCrypto::aesDecrypt(unsigned char *encMsg, size_t encMsgLen) {
  char **decMsg = NULL;
  aesDecrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCrypto::aesDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
  size_t decLen   = 0;
  size_t blockLen = 0;

  *decMsg = (char*)malloc(encMsgLen);
  if(*decMsg == NULL) return FAILURE;

  if(!EVP_DecryptInit_ex(aesDecryptCtx,
                         EVP_aes_256_cbc(),
                         NULL,
                         aesKey,
                         aesIV)) {
    return FAILURE;
  }

  if(!EVP_DecryptUpdate(aesDecryptCtx,
                        (unsigned char*)*decMsg,
                        (int*)&blockLen, encMsg,
                        (int)encMsgLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  if(!EVP_DecryptFinal_ex(aesDecryptCtx,
                          (unsigned char*)*decMsg + decLen,
                          (int*)&blockLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  (*decMsg)[decLen] = '\0';

  EVP_CIPHER_CTX_cleanup(aesDecryptCtx);

  return encMsgLen;
}


int SslCrypto::init(size_t rsaKeyLen, size_t aesKeyLen) {

  if (encSide == SIDE_CLIENT) {
    //FIXME: genTestClientKey(DEFAULT_RSA_KEYLEN);
  }

  // Initalize contexts
  rsaEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  aesEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

  rsaDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  aesDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

  if(rsaEncryptCtx == NULL || aesEncryptCtx == NULL ||
     rsaDecryptCtx == NULL || rsaDecryptCtx == NULL) {
    return FAILURE;
  }

  // Init these here to make valgrind happy
  EVP_CIPHER_CTX_init(rsaEncryptCtx);
  EVP_CIPHER_CTX_init(aesEncryptCtx);

  EVP_CIPHER_CTX_init(rsaDecryptCtx);
  EVP_CIPHER_CTX_init(aesDecryptCtx);

  // Init RSA
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if(EVP_PKEY_keygen_init(ctx) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, (int)rsaKeyLen) <= 0) {
    return FAILURE;
  }

  if(!(keypair = EVP_PKEY_new())) {
    return FAILURE;
  }

  EVP_PKEY_CTX_free(ctx);

  rsaSymKey = (unsigned char*)malloc(rsaKeyLen/8);
  rsaIV = (unsigned char*)malloc(EVP_MAX_IV_LENGTH);

  // Init AES
  aesKey = (unsigned char*)malloc(aesKeyLen/8 + EVP_MAX_IV_LENGTH);
  aesIV = (unsigned char*)malloc(aesKeyLen/8);

  if(rsaSymKey == NULL || rsaIV == NULL || aesKey == NULL || aesIV == NULL) {
    return FAILURE;
  }

  if(!EVP_BytesToKey(EVP_aes_256_cbc(),
                     EVP_sha1(),
                     (const unsigned char*)SALT,
                     (const unsigned char*)AES_KEY_PASS,
                     strlen(AES_KEY_PASS),
                     AES_ROUNDS,
                     aesKey,
                     aesIV)) {
    return FAILURE;
  }

  return SUCCESSFUL;
}


int SslCrypto::setKey(std::string& keyPath, int side)
{
  FILE *file = fopen(keyPath.c_str(),"r");
  if (file) {
    if(side == SIDE_SERVER) {
      RSA* privKey = PEM_read_RSAPrivateKey(file, &privKey, NULL, NULL);
      EVP_PKEY_assign_RSA(keypair, privKey);
    } else {
      RSA* pubKey = PEM_read_RSA_PUBKEY(file, &pubKey, NULL, NULL);
      EVP_PKEY_assign_RSA(publicKey, pubKey);
    }
    return SUCCESSFUL;
  }
  return FAILURE;
}


void TlsServer::run()
{
  /* Init OpenSSL */
  SSL_library_init();
  ERR_load_crypto_strings();
  ERR_load_SSL_strings();
  OpenSSL_add_all_algorithms();

  /* Init a ZMQ LPC client */
  zmq::context_t zctx(1);
  LazyPirateClient zlpc(zctx, internalServiceUri, getTimeout());

  // Creating a SSL context
  SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
  if(!ctx) {
    ERR_print_errors_fp(stderr);
    errorMsg = (boost::format("Failed getting SSL_CTX.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  // Set the context with the private key and the certificate
  if (!SSL_CTX_use_certificate_file(ctx, certificate.c_str(), SSL_FILETYPE_PEM)
      || !SSL_CTX_use_PrivateKey_file(ctx, privateKey.c_str(), SSL_FILETYPE_PEM)
      || !SSL_CTX_check_private_key(ctx)) {
    errorMsg = (boost::format("Failed setting SSL_CTX.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Setup the SSL BIO as server */
  SSL* ssl;
  BIO* sslBio = BIO_new_ssl(ctx,0);
  BIO_get_ssl(sslBio, &ssl);
  if(!ssl) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }

  /* Don't want any retries */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Create the buffering BIO and add to chain */
  BIO* bio4Buffer = BIO_new(BIO_f_buffer());
  sslBio = BIO_push(bio4Buffer, sslBio);

  /* Set the binding address. Here 0.0.0.0:mport*/
  char* portStr = const_cast<char*>(vishnu::convertToString(listeningPort).c_str());
  BIO* acceptBio = BIO_new_accept(portStr);

  BIO_set_accept_bios(acceptBio, sslBio);
  if(BIO_do_accept(acceptBio) <= 0) {
    errorMsg = (boost::format("Failed setting up accept BIO.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    throw SystemException(ERRCODE_COMMUNICATION, errorMsg);
  }


  /* Now wait for incoming connections */
  char msg[MAX_SSL_MSG];
  while (1) {
    if(BIO_do_accept(acceptBio) <= 0) { /* Wait for new connection */
      errorMsg = (boost::format("Failed connecting a client.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      std::cerr << errorMsg << "\n";
      continue;
    }

    sslBio = BIO_pop(acceptBio);
    BIO_free_all(acceptBio);

    /*  First, do handshake */
    if(BIO_do_handshake(sslBio) <= 0) {
      errorMsg = (boost::format("SSL handshake.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      std::cerr << errorMsg << "\n";
      continue;
    }

    /* Then receive message */
    int len = BIO_gets(sslBio, msg, MAX_SSL_MSG);
    if (len <= 0) {
      errorMsg = (boost::format("[ERROR] Empty message reveived.\n%1%"
                                )%ERR_error_string(ERR_get_error(), NULL)).str();
      std::cerr << errorMsg << "\n";
      continue;
    }

    /* Forward the message to the service handler */
    zlpc.send(std::string(msg, len));

    /* Retrive result and forward it to the client */
    std::string reply = zlpc.recv();
    BIO_puts(sslBio, reply.c_str());
  }

  /* Flush buffer and free BIO object */
  BIO_flush(sslBio);
  BIO_free_all(sslBio);
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
  BIO* sslBio = BIO_new_ssl_connect(sslctx);
  BIO_get_ssl(sslBio, &ssl);
  if(!ssl) {
    errorMsg = (boost::format("Can't locate SSL pointer.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Don't want any retries */
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  /* Connecting to server */
  std::string addr = (boost::format("%1%:%2%")%serverAddr%serverPort).str();
  BIO_set_conn_hostname(sslBio, const_cast<char*>(addr.c_str()));
  if(BIO_do_connect(sslBio) <= 0) {
    errorMsg = (boost::format("Failed connecting to server.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Verify certificate */
  if(SSL_get_verify_result(ssl) != X509_V_OK) {
    errorMsg = (boost::format("Failed verifying the server certificate.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Do the handshake */
  if(BIO_do_handshake(sslBio) <= 0) {
    errorMsg = (boost::format("Failed establishing SSL connection.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Retrieve server certificate */
  X509* peerCert = SSL_get_peer_certificate(ssl);;
  if (peerCert == NULL) {
    errorMsg = (boost::format("Failed getting peer certificate key.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Retriving server public key */
  peerPublicKey = X509_get_pubkey(peerCert);
  if (peerPublicKey == NULL) {
    errorMsg = (boost::format("Failed getting server public key.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  /* Now send the request data */
  int len = BIO_puts(sslBio, reqData.c_str());
  if (len != reqData.size()) {
    errorMsg = (boost::format("Can't send message to server.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  } else {
    // For logging
    std::cerr << boost::format("[INFO] %1% bytes sent\n")%len;
  }

  char reply[MAX_SSL_MSG];
  len = BIO_read(sslBio, reply, MAX_SSL_MSG);
  if (len <= 0) {
    errorMsg = (boost::format("Empty message received.\n%1%"
                              )%ERR_error_string(ERR_get_error(), NULL)).str();
    return -1;
  }

  data = std::string(reply, len);
  /* Free BIO object */
  BIO_free_all(sslBio);


  return 0;
}
