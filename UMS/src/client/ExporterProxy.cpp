
#include <iostream>
#include <string>
#include "ExporterProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "QueryProxy.hpp"
#include "UMSServices.hpp"
#include "UserException.hpp"

using namespace vishnu;

/**
 * \param session The object which encapsulates the session information
 * \brief Constructor
 */
ExporterProxy::ExporterProxy(const SessionProxy& session): msessionProxy(session) {
}

/**
 * \brief Function to export all the commands launched by a user during a session
 * \param oldSessionId The identifier of a session closed to export
 * \param filename The path of the output file containing the Vishnu shell commands
 * \return raises an exception on error
 */
int
ExporterProxy::exportCmd(const std::string& oldSessionId,
                         const std::string& filename) {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string errorInfo;
  std::string fileContent;

  std::string serviceName = SERVICES_UMS[EXPORT];

  profile = diet_profile_alloc(serviceName.c_str(), 3);
  sessionKey = msessionProxy.getSessionKey();
//
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(profile,1, oldSessionId.c_str())) {
    msgErrorDiet += "with oldSessionId parameter "+oldSessionId;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(profile,2, filename.c_str())) {
    msgErrorDiet += "with filename parameter "+filename;
    raiseCommunicationMsgException(msgErrorDiet);
  }

//
  if(diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_string_get(profile,3, fileContent);

  //To put the file content on the corresponding file
  std::ofstream ofile(filename.c_str());
  //If the file is open
  if (ofile.is_open()) {
    ofile << fileContent;
    ofile.close();
  }
  else {
    throw UserException(ERRCODE_INVALID_PARAM, "Error opening file: " + filename);
  }
  diet_profile_free(profile);
  return 0;
}

/**
 * \brief Destructor
 */
ExporterProxy::~ExporterProxy() {
}
