/**
 * \file internalApi.cc
 * \brief This file presents the internal api of UMS
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001 
*/

#include "internalApi.hh"


/**
 * \brief Function to solve the service sessionConnect 
 * \fn    void solveSessionConnect(diet_profile_t* pb);
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionConnect(diet_profile_t* pb) {  

  char* userId = NULL;
  char* password = NULL;
  char* clientKey = NULL;
  char* clientHostname = NULL;
  char* options = NULL;
  
  //TODO : faire des test sur les retours de if !diet_strinq_get throw ... 
  diet_string_get(diet_parameter(pb,0), &userId, NULL);
  std::cout<<"userID:"<<userId<<std::endl;
  diet_string_get(diet_parameter(pb,1), &password, NULL);
  std::cout<<"password:"<<password<<std::endl;
  diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
  std::cout<<"clientKey:"<<clientKey<<std::endl;
  diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
  std::cout<<"clientHostname:"<<clientHostname<<std::endl;
  diet_string_get(diet_parameter(pb,4), &options, NULL);
  
  UserServer userServer = UserServer(std::string(userId), std::string(password));
  MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
  SessionServer sessionServer("");
          
     try { 
     std::string empty("");  
     sessionServer.connectSession(userServer, machineClientServer, options);
     diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
     diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
  
     } catch (SystemException& e) {
	std::string empty("");
	std::string errorInfo = e.getMsg()+"==>";
	errorInfo.append(e.what());
	
	std::cout << "errorInfo: " << errorInfo <<std::endl;
	
	diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
	diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
    }
   
  
}
/**
 * \brief Function to solve the service sessionReconnect 
 * \fn    void solveSessionReconnect
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionReconnect(diet_profile_t* pb) {  
 
 char* userId = NULL;
 char* password = NULL;
 char* clientKey = NULL;
 char* clientHostname = NULL;
 char* sessionId = NULL;
   
  
 diet_string_get(diet_parameter(pb,0), &userId, NULL);
 std::cout<<"userID:"<<userId<<std::endl;
 diet_string_get(diet_parameter(pb,1), &password, NULL);
 std::cout<<"password:"<<password<<std::endl;
 diet_string_get(diet_parameter(pb,2), &clientKey, NULL);
 std::cout<<"clientKey:"<<clientKey<<std::endl;
 diet_string_get(diet_parameter(pb,3), &clientHostname, NULL);
 std::cout<<"clientHostname:"<<clientHostname<<std::endl;
 diet_string_get(diet_parameter(pb,4), &sessionId, NULL);
 std::cout<<"sessionId:"<<sessionId<<std::endl;
 
 UserServer userServer = UserServer(std::string(userId), std::string(password));
 MachineClientServer machineClientServer =  MachineClientServer(std::string(clientKey), std::string(clientHostname));
 
 
 SessionServer sessionServer = SessionServer(std::string(""));
 
 sessionServer.getData().setSessionId(std::string(sessionId)); 
 std::cout<<"sessionId dans SessionServer:"<<sessionServer.getData().getSessionId()<<std::endl;
 
 try { 
     std::string empty("");  
     sessionServer.reconnect(userServer, machineClientServer, std::string(sessionId));
     diet_string_set(diet_parameter(pb,5), strdup(sessionServer.getData().getSessionKey().c_str()), DIET_VOLATILE);
     diet_string_set(diet_parameter(pb,6), strdup(empty.c_str()), DIET_VOLATILE);
  
 } catch (SystemException& e) {
	std::string empty("");
	std::string errorInfo = e.getMsg()+"==>";
	errorInfo.append(e.what());
	
	std::cout << "errorInfo: " << errorInfo <<std::endl;	
	diet_string_set(diet_parameter(pb,5), strdup(empty.c_str()), DIET_VOLATILE);
	diet_string_set(diet_parameter(pb,6), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
   
}
/**
 * \brief Function to solve the service sessionClose 
 * \fn    int solveSessionClose(diet_profile_t* pb)
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \return raises an exception on error
 */
int 
solveSessionClose(diet_profile_t* pb) {
  
  char* _sessionKey = NULL;
  
  diet_string_get(diet_parameter(pb,0), &_sessionKey, NULL);
  
  SessionServer sessionServer = SessionServer(std::string(_sessionKey));
  try {
    std::string empty("");
    sessionServer.close();
    diet_string_set(diet_parameter(pb,1), strdup(empty.c_str()), DIET_VOLATILE);
   } catch (SystemException& e) {
	std::string empty("");
	std::string errorInfo = e.getMsg()+"==>";
	errorInfo.append(e.what());
	std::cout << "errorInfo: " << errorInfo <<std::endl;
	diet_string_set(diet_parameter(pb,1), strdup(errorInfo.c_str()), DIET_VOLATILE);
  } 
    
}

/**
* \brief Function to solve the service userCreate 
* \fn    int solveUserCreate(diet_profile_t* pb)
* \param pb is a structure which corresponds to the descriptor of a profile
* \return raises an exception on error
*/
int 
solveUserCreate(diet_profile_t* pb) {
 char* sessionKey = NULL;
 char* userSerialized = NULL;
 
 
 diet_string_get(diet_parameter(pb,0), &sessionKey, NULL);
 std::cout<<"sessionKey:"<< sessionKey <<std::endl;
 diet_string_get(diet_parameter(pb,1), &userSerialized, NULL);
 std::cout<<"User:"<< userSerialized <<std::endl;
 
 SessionServer sessionServer = SessionServer(std::string(sessionKey));
 UserServer userServer = UserServer(sessionServer);
 
 
  try {
    std::string empty("");
    //sessionServer.close();
    userServer.add(userSerialized);
    diet_string_set(diet_parameter(pb,2), strdup(empty.c_str()), DIET_VOLATILE);
    
   } catch (SystemException& e) {
	std::string empty("");
	std::string errorInfo = e.getMsg()+"==>";
	errorInfo.append(e.what());
	std::cout << "errorInfo: " << errorInfo <<std::endl;
	diet_string_set(diet_parameter(pb,2), strdup(errorInfo.c_str()), DIET_VOLATILE);
  }
 
 
 
 
}  
