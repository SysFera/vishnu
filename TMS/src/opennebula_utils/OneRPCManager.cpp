/*                                                                             #
# File        : OneRpcManager.cpp                                              #
# Copyright   : Rodrigue Chakode 2010 <rodrigue.chakode@gmail.com>             #
# Description : Class and header to request data from OpenNebula's XML-RPC API #
*/

#include "utilServer.hpp"
#include "OneRPCManager.hpp"

#define MAX_MESSAGE_SIZE 51200
#define XMLRPC_TRY try{
#define XMLRPC_CATCH } catch (girerr::error const ex) {\
  mrpcCallSucceeded = false; \
  mstringResult = std::string(ex.what()); \
  } catch (std::exception const ex) { \
  mrpcCallSucceeded = false; \
  mstringResult = std::string(ex.what()); \
  }

OneRPCManager::OneRPCManager(std::string url)
  : xmlrpc_c::clientSimple(),
    moneRpcUrl(url)
{
  initXmlRpcEnvironment();
}

OneRPCManager::~OneRPCManager()
{
  finalizeXmlRpcEnvironment();
}


/**
 * @brief initialize the encapsulated xmlrpc_c RPC handler
 */
void
OneRPCManager::initXmlRpcEnvironment()
{
  XMLRPC_TRY;

  mrpcEnv = new xmlrpc_env;
  xmlrpc_env_init(mrpcEnv);
  xmlrpc_limit_set(XMLRPC_XML_SIZE_LIMIT_ID, 1024*MAX_MESSAGE_SIZE);

  XMLRPC_CATCH
}

/**
 * @brief clear the encapsulated xmlrpc_c RPC handler
 */
void
OneRPCManager::finalizeXmlRpcEnvironment()
{
  XMLRPC_TRY;

  xmlrpc_env_clean(mrpcEnv);
  delete mrpcEnv;

  XMLRPC_CATCH
}


/**
 * @brief Add a parameter value
 * @param param The parameter as xmlrpc_c::value
 */
void
OneRPCManager::addParam(xmlrpc_c::value param)
{
  XMLRPC_TRY;

  this->mrequestParams.add(param);

  XMLRPC_CATCH
}

/**
 * @brief Add a string parameter
 * @param param The parameter as xmlrpc_c::value
 */
void
OneRPCManager::addParam(const std::string& param)
{
  addParam(xmlrpc_c::value_string(param));
}

/**
 * @brief Add a int parameter
 * @param param The parameter as xmlrpc_c::value
 */
void
OneRPCManager::addParam(int param)
{
  addParam(xmlrpc_c::value_int(param));
}

/**
 * @brief Add a boolean parameter
 * @param param The parameter as xmlrpc_c::value
 */
void
OneRPCManager::addParam(bool param)
{
  addParam(xmlrpc_c::value_boolean(param));
}

/**
 * @brief Execute the encapsulated xmlrpc_c request
 */
void
OneRPCManager::execute(void)
{
  XMLRPC_TRY;

  xmlrpc_c::value requestResult;
  call(moneRpcUrl, method, mrequestParams, &requestResult);

  xmlrpc_c::value_array value_objs = xmlrpc_c::value_array(requestResult);
  std::vector<xmlrpc_c::value> const values(value_objs.vectorValueValue());
  //xmlrpc_c::value_boolean returnStatus = static_cast<xmlrpc_c::value_boolean>(values[0]);
  mrpcCallSucceeded = static_cast<bool>(static_cast<xmlrpc_c::value_boolean>(values[0]));

  if( values.size() > 1 ) {
    if(values[1].type() == xmlrpc_c::value::TYPE_INT) {
      if (mrpcCallSucceeded) {
        mintResult = static_cast<int>( static_cast<xmlrpc_c::value_int>(values[1]) );
      }
    } else {
      mstringResult = static_cast<std::string>( static_cast<xmlrpc_c::value_string>(values[1]));
    }
  }
  XMLRPC_CATCH
}

/**
 * @brief Return the string used for authenticating again OpenNebula
 * @return
 */
void
OneRPCManager::setSecretOneAuthChain(void)
{

  std::string oneUser = "";
  std::string clearOneUserPass = "";
  std::string clearOneAuthChain = "";
  int rc = getOneAuthChain(clearOneAuthChain);
  if ( rc == 0 ) {
    rc = splitOneAuthChain(clearOneAuthChain, oneUser, clearOneUserPass);
    if( rc == 0 ) {
      std::string sha1Pass = sha1Digest( clearOneUserPass );
      this->msecretOneAuthChain = oneUser + ":" + sha1Pass;
    } else {
      LOG("[ERROR] Wrong format for auth token, must be <username>:<passwd>", 4);
    }
  }
}


/**
 * @brief Split a one_auth chain to have the username and the password
 * @param oneAuthChain The one_auth chain in the form of username:password
 * @param username The resulting username
 * @param password The resulting password
 * @return The number of fields separated by colon ':'
 */
int
OneRPCManager::splitOneAuthChain(const std::string oneAuthChain, std::string& username, std::string& password)
{
  int rc = -1;
  size_t pos=oneAuthChain.find(":");
  if (pos != std::string::npos) {
    username = oneAuthChain.substr(0,pos);
    password = oneAuthChain.substr(pos+1);
    rc = 0;
  }

  return rc;
}

/**
 * @brief Read the one_auth file and to get the authentication chain
 * @param oneAuthChain The resulting auth chain
 * @return
 */
int
OneRPCManager::getOneAuthChain(std::string & oneAuthChain)
{
  int rc = -1;

  // Read $ONE_AUTH file.
  const char* oneAuthEnv = getenv("ONE_AUTH");

  if (! oneAuthEnv) {
    // If $ONE_AUTH doesn't exist, read $HOME/.one/one_auth
    struct passwd * pwEnt;

    pwEnt = getpwuid(getuid());

    if ((pwEnt != NULL) && (pwEnt->pw_dir != NULL)) {
      std::string oneAuthFile = pwEnt->pw_dir;
      oneAuthEnv = oneAuthFile.append("/.one/one_auth").c_str();
    } else {
      LOG("[ERROR] Could not get one_auth file location", 4);
    }
  }

  std::ifstream file;
  file.open(oneAuthEnv);

  if (file.good()) {
    getline(file, oneAuthChain);
    if (file.fail()) {
      LOG("[ERROR] Error reading file: " + std::string(oneAuthEnv), 4);
    } else {
      rc = 0;
    }
  } else {
    LOG("[ERROR] Could not open file: "+ std::string(oneAuthEnv), 4);
  }

  file.close();

  return rc;
}


/**
 * @brief sha1Digest
 * @param oneUserPass
 * @return
 */
std::string
OneRPCManager::sha1Digest(const std::string& oneUserPass)
{
  EVP_MD_CTX     mdctx;
  unsigned char  mdValue[EVP_MAX_MD_SIZE];
  unsigned int   mdLen;
  std::ostringstream  oss;

  EVP_MD_CTX_init(&mdctx);
  EVP_DigestInit_ex(&mdctx, EVP_sha1(), NULL);

  EVP_DigestUpdate(&mdctx, oneUserPass.c_str(), oneUserPass.length());

  EVP_DigestFinal_ex(&mdctx, mdValue, &mdLen);
  EVP_MD_CTX_cleanup(&mdctx);

  for(unsigned int i = 0; i<mdLen; ++i) {
    oss << std::setfill('0') << std::setw(2) << std::hex << std::nouppercase
        << (unsigned short) mdValue[i];
  }

  return oss.str();
}

