
#include <iostream>
#include <string>
#include "ExporterProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "QueryProxy.hpp"
#include "IMSServices.hpp"
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
 * \param options The data structure which encapsulates the o object export
 * \return raises an exception on error
 */
int
ExporterProxy::exportCmd(const std::string& oldSessionId,
                         const std::string& filename,
                         IMS_Data::ExportOp options) {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string errorInfo;
  std::string fileContent;

  std::string serviceName = SERVICES_IMS[INT_EXPORTCOMMANDS];

  profile = diet_profile_alloc(serviceName.c_str(), 3, 3, 5);
  sessionKey = msessionProxy.getSessionKey();
//
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(profile,1, oldSessionId.c_str())) {
    msgErrorDiet += "with oldSessionId parameter "+oldSessionId;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(profile,2, filename.c_str())) {
    msgErrorDiet += "with filename parameter "+filename;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object
  std::string exportOptToString =  _ser.serialize_str(const_cast<IMS_Data::ExportOp_ptr>(&options));

  if (diet_string_set(profile,3, exportOptToString.c_str())) {
    msgErrorDiet += "with Export options parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(profile,4);
  diet_string_set(profile,5);
//
  if(!diet_call(profile)) {

    if(diet_string_get(profile,4, fileContent)){
      msgErrorDiet += " by receiving the file content";
      raiseDietMsgException(msgErrorDiet);
    }

    if(diet_string_get(profile,5, errorInfo)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

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
