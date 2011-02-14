#include "utilsClient.hpp"

void sendErrorMsg(std::string msg) {
    std::string msgWithHead = "Error: "+msg;
    UMSVishnuException e(1, msgWithHead);
    throw e;
}

void checkErrorMsg(std::string msg) {

  if(msg.size() > 0 ) {
    char* codeInString = strtok(strdup(msg.c_str()), "#");
  if(codeInString!=NULL) {
    std::istringstream isCode(codeInString);
    int ret;
    isCode >> ret;
    char* message = strtok(NULL, codeInString);
    if(message!=NULL) {
      std::cout << "ERROR: code d'erreur de retour = " << ret << std::endl;
      std::cout << "ERROR: " << message << std::endl;
      UMSVishnuException e(ret, message);
      throw e;
      }
    }
 }
}

