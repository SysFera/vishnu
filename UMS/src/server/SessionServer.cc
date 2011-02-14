/**
* \file SessionServer.cpp
* \brief This file implements the Class which manipulates VISHNU sessions on server side.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2011
*/

#include "SessionServer.hh"
/**
* \brief Constructor
* \fn SessionServer()
*/
SessionServer::SessionServer() {
  DbFactory factory;
  msession.setSessionKey("");
  mdatabaseVishnu = factory.getDatabaseInstance(); 
}
/**
* \brief Constructor
* \fn SessionServer(std::string sessionKey);
* \param sessionKey The session key of the session
*/
SessionServer::SessionServer(std::string sessionKey) {
  DbFactory factory;
  msession.setSessionKey(sessionKey);
  mdatabaseVishnu = factory.getDatabaseInstance(); 
}
/**
* \brief Constructor
* \fn SessionServer(const UMS_Data::Session& session)
* \param session The session data structure
*/ 
SessionServer::SessionServer(const UMS_Data::Session& session):msession(session) {
  DbFactory factory;
  mdatabaseVishnu = factory.getDatabaseInstance(); 
}
/**
* \brief Function to connect the session
* \fn int connectSession(UserServer, MachineClientServer, ConnectOptions* ) 
* \param user The object which manipulates user information
* \param host The object which manipulates client machine information
* \param connectOpt The options data structure for connection
* \return raises an exception on error
*/
int 
SessionServer::connectSession(UserServer user, MachineClientServer host, UMS_Data::ConnectOptions* connectOpt) {
  std::string numSubstituteUserId;
  std::string numUserIdToconnect;
  
  try {
    //if the user exist 
    if (user.exist()) {
      solveConnectionMode(connectOpt, user);
      //if a user to substitute is defined
      
      if (connectOpt->getSubstituteUserId().size() != 0) {
	std::cout << "USer to substitute:" << connectOpt->getSubstituteUserId() << std::endl;	
	// if the user is admin
	if (user.isAdmin()) {
	  //TODO faire une fonction checklogin qui vérifie 
	  //que le userID appartient a un utilisateur qui exist et qui n'est pas blocked
	  numSubstituteUserId = user.getAttribut("where \
	  userid='"+connectOpt->getSubstituteUserId()+"'");
	  
	  //If the user to substitute exist
	  if (user.existuserId(connectOpt->getSubstituteUserId())) {
	  //if (numSubstituteUserId.size() != 0) {
	    
	    //if (user.getAttribut("where userid='"+user.getData().getUserId()+"'")) {
	      numUserIdToconnect = numSubstituteUserId;
	    //} 
	  } //End If the user to substitute exist
	  else {
	    UMSVishnuException e(4, "The user to substitute is unknown!");
	    throw e;
	  }  
	} // END if the user is admin
	else {
	  UMSVishnuException e(4, "The substitution is an admin option!");
	  throw e;
	}
     } //End if connectOpt->getSubstituteUserId().size() != 0
    
     //if there is not a numSubstituteUserId
     if (numUserIdToconnect.size() == 0) {
	numUserIdToconnect = user.getAttribut("where userid='"+user.getData().getUserId()+"'\
	and pwd='"+user.getData().getPassword()+"'"); 
     } //END if There is not a numSubstituteUserId 
     /*else {
	generateSessionKey(connectOpt->getSubstituteUserId());
	generateSessionId(connectOpt->getSubstituteUserId());
    }*/
    
    generateSessionKey(user.getData().getUserId());
    generateSessionId(user.getData().getUserId()); 
    
    std::cout <<" After generation SessionKey:" << msession.getSessionKey()<< std::endl;
    std::cout <<" After generation SessionId:" << msession.getSessionId()<< std::endl;
  
    host.recordMachineClient();
    recordSessionServer(host.getId(), numUserIdToconnect);
	     
    //TODO: machineServer et commanderServer à enregistrer dans la base de données donc créer un commanderServer
    
    } // END if the user exist 
    else {
      UMSVishnuException e(4, "The user is unknwon");
      throw e;
    }
  
  } //END try 
  catch (SystemException& e) {
	throw e;
  }
  return 0;
}//END: connectSession(UserServer, MachineClientServer, ConnectOptions*)
/**
* \brief Function to reconnect the session
* \fn int reconnect(UserServer, MachineClientServer, std::string sessionId) 
* \param user The object which manipulates user information
* \param host The object which manipulates client machine information
* \param sessionId The session identifier
* \return raises an exception on error
*/   
int 
SessionServer::reconnect(UserServer user, MachineClientServer host, std::string sessionId) {
 
  std::string key;
  int existSessionKey = 0; 
  int state;
  
  msession.setSessionId(sessionId);
  
  try {
    //If the user exists
    if (user.exist()) {
      state = getState(true);
      // -1 is an error code of getState when nohting has found
      if (state != -1) {
	//if the session is active
	if (state == 1) {	  
	  //if user is an admin
	  if (user.isAdmin()) {
	    existSessionKey = getSessionkey("", "", true);  
	  } //END if user is an admin
	  else {
	    existSessionKey = getSessionkey(host.getId(), user.getAttribut("where userid='"+user.getData().getUserId()+"'\
					  and pwd='"+user.getData().getPassword()+"'"));
	  }
	  //if there is no session key with the previous parameters
	  if (existSessionKey == -1) {
	    UMSVishnuException e(4, "This session has not been opened with this machine for the corresponding user");
	    throw e;
	  }
	}//if the session is active
	else {
	  UMSVishnuException e(4, "The reconnection is impossible because the session is closed");
	  throw e;
	}	  
      }//END if state != -1
      else {
	UMSVishnuException e(4, "The sessionId is unrecognized");
	throw e;
      }
    } //END IF user.exist
    else {
      UMSVishnuException e(4, "The user is unknwon");
      throw e;
    }
  }//END Try 
  catch (SystemException& e) {
    throw e;
  }
  return 0;
}//END: reconnect(UserServer, MachineClientServer, string sessionId)
/**
* \brief Function to close the session
* \fn int close()
* \return raises an exception on error
*/ 
int SessionServer::close() {
  
  std::string sqlCommand = "UPDATE vsession SET state=0 WHERE sessionkey='";
  int state;
  
  try {
    UserServer user = UserServer(SessionServer(msession.getSessionKey()));
    
    //The init function initializes login and password using the sessionKey
    user.init();	
      //If The user exist
    if (user.exist()) {
      state = getState();
      //if the session is not already closed
      if (state != 0) {
      //TODO: if no running commands ==> vérifier dans la table commande qu'il n'y a pas de commande à status
	//active
	sqlCommand.append(msession.getSessionKey()+"';");
	sqlCommand.append("UPDATE vsession SET closure=CURRENT_TIMESTAMP\
	WHERE sessionkey='"+msession.getSessionKey()+"';");	
	mdatabaseVishnu->process(sqlCommand.c_str());		
      } //if the session is not already closed
      else {
	UMSVishnuException e (4, "The session key is expired. The session is already closed");
	throw e;
      }  
    } //END If The user exist
  } 
  catch (SystemException& e) {
    throw e;
  }
  return 0;
}//END: close()

/**
* \brief Destructor
* \fn ~SessionServer()
*/
SessionServer::~SessionServer() {
}
/**
* \brief Function to get the session information
* \fn UMS_Data::Session getData()
* \return  The session data structure
*/ 
UMS_Data::Session 
SessionServer::getData() {
  return msession;
}

/**
* \brief Function to get session information from the database vishnu
* \fn getAttribut(std::string condition, std::string attrname);
* \param condition The condition of the select request
* \param attrname the name of the attribut to get
* \return the value of the attribut or empty string if no results
*/
std::string 
SessionServer::getAttribut(std::string condition, std::string attrname) {
  DatabaseResult* result;
  std::vector<std::string>::iterator ii;
  
  std::string sqlCommand("SELECT "+attrname+" FROM vsession "+condition);
  std::cout <<"SQL COMMAND:"<<sqlCommand;
    
  try {
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
  } 
  catch (SystemException& e) {
    throw e;
  }
  //TODO: Dans databaseResult rajouter ce bout de code dans une fonction getFirstElement
  if (result->getNbTuples() != 0) {
    result->print();
    std::vector<std::string> tmp = result->get(0);  
    ii=tmp.begin();
    return (*ii);
  } 
  else {
    return "";
  }
}
/**
* \brief Function to generate the session key
* \fn    int generateSessionKey(std::string salt)
* \param salt This string is used to perturb the algorithm
* \return an encrypted message registered on the session data structure
*/
int 
SessionServer::generateSessionKey(std::string salt) {
  //TODO: enlever timeval et remove malloc	
  struct timeval  tv;
  struct timezone tz;
  struct tm *tm;
  float timeMilliseconde;
  char *clef = (char *) malloc(100 * sizeof(char));
  char *tmp = (char *) malloc(200 * sizeof(char)); 
  
  srand(time(NULL));
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  timeMilliseconde = tm->tm_hour * 3600 * 1000 + 
  tm->tm_min * 60 * 1000 + tm->tm_sec * 1000 + tv.tv_usec/1000;
  
  //current time
  sprintf(clef,"%d-%d-%d-%d:%d:%d:%d (~%0.1f ms)", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec, timeMilliseconde*rand());
    
  //for the md5 encryption
  std::string tmpSalt = "$1$" + salt + "$";
  
  //For SHA1-512 encryption
  std::string globalSalt = "$6$"+std::string(crypt(salt.c_str(), tmpSalt.c_str()))+"$";
  
  //SHA1-512 encryption of the salt encrypted using the md5 and the current time as the clef
  std::cout << "SessionKey generated:" << std::string(crypt(clef,globalSalt.c_str())+5) <<std::endl;
  
  msession.setSessionKey(std::string(crypt(clef,globalSalt.c_str())+5));
  
  return 0;
}
/**
* \brief Function to generate the session identifier
* \fn    generateSessionId(string userId)
* \param userId the userId of the owner of the session
* \return record the the session id on the session data structure
*/ 
int 
SessionServer::generateSessionId(std::string userId) {
  //TODO: enlever timeval et remove malloc
  struct timeval  tv;
  struct timezone tz;
  struct tm *tm;
  char *clef = (char *) malloc(100 * sizeof(char));
  
  srand(time(NULL));
  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);
	
  sprintf(clef,"-%d-%d-%d-%d:%d:%d:%d:%d", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900, tm->tm_hour, 
	  tm->tm_min, tm->tm_sec, (int)tv.tv_usec, rand()%1000 + 1);
   
  userId.append(std::string(clef));
  msession.setSessionId(userId);
  
  return 0;
} 
/**
* \brief Function to record the session on the database
* \fn    recordSessionServer(string idmachine, string iduser)
* \param idmachine the database number id of the client machine
* \param iduser the database number id of the owner of the session
* \return record the the session id on the session data structure
*/ 
int 
SessionServer::recordSessionServer(std::string idmachine, std::string iduser) {
  
  std::string sqlInsert = "insert into vsession\
  (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, \
  creation, sessionKey, state, closepolicy, timeout) values "; 
  
  std::string values = std::string("('" +msession.getSessionId()+"',"+idmachine+","+iduser+",\
  CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '"+msession.getSessionKey()+"',");
    
  values.append(convertToString(msession.getStatus())+",");
  values.append(convertToString(msession.getClosePolicy())+",");
  values.append(convertToString(msession.getTimeout())+")");
    
  sqlInsert.append(values);
  std::cout << "SQL COMMAND:"<< sqlInsert << std::endl;
    
  try {
    mdatabaseVishnu->process(sqlInsert.c_str());
  } 
  catch (SystemException& e) {
    throw e;
  }
} 
/**
* \brief Function to check the session on database
* \fn bool exist(bool flagSessionId)
* \param flagSessionId A flag to check the session id
* \return true if the session key (or sessionId) exists else false
*/
bool 
SessionServer::exist(bool flagSessionId) {
  
  if (flagSessionId) {
    try {  
      return (getState(true) != -1);  
    } 
    catch (SystemException& e) {
      throw e;
    } 
  }
  else {
    try {  
      return (getState(true) != -1);  
    } 
    catch (SystemException& e) {
	throw e;  
    }
  }
} 


/**
* \brief Function to check the session on database
* \fn int getState(bool flagSessionId)
* \param flagSessionId A flag to check the session identifier
* \return the session status
*/
int 
SessionServer::getState(bool flagSessionId) {
 
  std::string sessionState;
 
  try {
    if (flagSessionId) {
	//sqlCommand = "SELECT state FROM vsession where vsessionid='"+msession.getSessionId()+"'";
      sessionState = getAttribut("where vsessionid='"+msession.getSessionId()+"'","state");
    }
    else {
	//sqlCommand = "SELECT state FROM vsession where sessionkey='"+msession.getSessionKey()+"'";
      sessionState = getAttribut("where sessionkey='"+msession.getSessionKey()+"'","state");
      
    }
    
    if (sessionState.size() != 0) {
    return convertToInt(sessionState);
    }
    else {
      return -1;
    }
    
  }
  catch (SystemException& e) {
    throw e;
  }
 
   
}
/**
* \brief Function to check the session on database
* \fn int getSessionkey(string idmachine, string iduser, bool flagAdmin)
* \param idmachine the database number id of the client machine
* \param iduser the database number id of the owner of the session
* \param flagAdmin A flag for an admin to get the session key from another user
* \return 0 on success and -1 if there is no results
*/ 
int 
SessionServer::getSessionkey(std::string idmachine, std::string iduser, bool flagAdmin) {

  try {
    std::string key = "";
    //if the user is not an admin, the machine and the userid are checked
    if (!flagAdmin) {
      key = getAttribut("where vsessionid='"+msession.getSessionId()+"' \
      and clmachine_numclmachineid="+idmachine+" and users_numuserid="+iduser);
    }
    else {
      //sqlCommand = "SELECT sessionkey FROM vsession where vsessionid='"+msession.getSessionId()+"'";  
      key = getAttribut("where vsessionid='"+msession.getSessionId()+"'");
    }
    std::cout <<"getSessionkey function:"<< key << std::endl;
    
    if (key.size() != 0) {
      msession.setSessionKey(key);
      return 0;
    }
    else {
      return -1;
    }
  }
  catch (SystemException& e) {
    throw e;
  }
}
 

/**
* \brief Function to solve the session connection parameters
* \fn solveConnectionMode(UMS_Data::ConnectOptions* connectOpt, UserServer user)
* \param connectOpt the connection parameters
* \param user the object which manipulates user information
* \return the connection parameters are registered on the session data structure
*/
int 
SessionServer::solveConnectionMode(UMS_Data::ConnectOptions* connectOpt, UserServer user) {
  
  OptionValueServer optionValueServer;
  
  switch (connectOpt->getClosePolicy()) {
    ////The closure mode is undefined
    case 0: 
      msession.setClosePolicy(optionValueServer.getClosureInfo(user.getAttribut("\
      where userid='"+user.getData().getUserId()+"'")));
	//If the policy is not 2 (CLOSE_ON_DISCONNECT)
      if (msession.getClosePolicy() != 2) { 
	msession.setTimeout(optionValueServer.getClosureInfo(user.getAttribut("\
	where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT"));
      } 
    break;
    //The closure mode is close on timeout
    case 1: 
      msession.setClosePolicy(1);
      if (connectOpt->getSessionInactivityDelay() != 0) {
	msession.setTimeout(connectOpt->getSessionInactivityDelay());
      } //END if the timeout is defined
      else {
	msession.setTimeout(optionValueServer.getClosureInfo(user.getAttribut("\
	where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT"));
      }
    break;
    //The closure mode is close on disconnect
    case 2: 
      msession.setClosePolicy(2); 
    break;
  }  
}
  
/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
