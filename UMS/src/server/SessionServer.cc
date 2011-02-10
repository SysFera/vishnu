/*#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>*/
#include <time.h>
#include <sys/time.h>

#include <exception>
#include "SystemException.hh"
#include "UMSVishnuException.hh"
#include "SessionServer.hh"
#include "utilServer.hh"
#include "OptionValueServer.hh"

SessionServer::SessionServer() {
  msession.setSessionKey("");
}

SessionServer::SessionServer(std::string sessionKey) {
  msession.setSessionKey(sessionKey);
  mdatabaseVishnu = factory.getDatabaseInstance(); 
}
 
SessionServer::SessionServer(const UMS_Data::Session& session):msession(session) {
}

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
  
  } catch (SystemException& e) {
	throw e;
  }
  return 0;
}
 
int SessionServer::reconnect (UserServer user, MachineClientServer host, std::string sessionId) {
 
 std::string key;
 int existSessionKey = 0; 
 int state;
 
 msession.setSessionId(sessionId);
 
 try {
    if (user.exist()) {
        state = getState(true);
	
	  // -1 is an error code of getState when nohting has found
	  if (state != -1) {
	    
	    if (state == 1) {
	        
	        if (user.isAdmin()) {
		  existSessionKey = getSessionkey("", "", true);  
		} //END if user is admin
		else {
		existSessionKey = getSessionkey(host.getId(), 
						user.getAttribut("where userid='"+user.getData().getUserId()+"'\
						and pwd='"+user.getData().getPassword()+"'")
	                                        );
		}
		
		if (existSessionKey == -1) {
		    UMSVishnuException e(4, "This session has not been opened with this machine for the corresponding user");
		    throw e;
		}
	    }//END IF state = 1;
	    else {
		//if (state == 0) {
		  UMSVishnuException e(4, "The reconnection is impossible because the session is closed");
		  throw e;
		/*} //END If state = 0
		else {
		  UMSVishnuException e(4, "The state associated to the session is incorrect");
		  throw e;
		}*/
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
 
 } catch (SystemException& e) {
	    throw e;
	}
 
 return 0;
}
 
int SessionServer::close() {
  
  int state;
  try {
	
	UserServer user = UserServer(SessionServer(msession.getSessionKey()));
	//The init function initializes login and password using the sessionKey
	user.init();	
	
	//We check here if all user information is ok (exist?, blocked?, deleted? passwordState)
       if (user.exist()) {
		
	state = getState();
	
	  //if the session is not already closed
	  if (state != 0) {
	      //TODO: if no running commands ==> vérifier dans la table commande qu'il n'y a pas de commande à status
	      //active
	        
		      std::string sqlCommand("UPDATE vsession SET state=0 WHERE sessionkey='"+msession.getSessionKey()+"';");
		      sqlCommand.append("UPDATE vsession SET closure=CURRENT_TIMESTAMP WHERE sessionkey='"+msession.getSessionKey()+"';");
		      
		      
		      
			//TODO: réflechir sur une transaction ici seul pb 
			  mdatabaseVishnu->process(sqlCommand.c_str());
		 
		
	  } //END IF STATE != 0 
	  else {
	    UMSVishnuException e (4, "The session key is expired. The session is already closed");
	    throw e;
	  }
	  
	} //END IF The user exist
	
  } catch (SystemException& e) {
		throw e;
  }
  return 0;
}
 
SessionServer::~SessionServer() {
}
 
UMS_Data::Session SessionServer::getData()
{
	return msession;
}
 
int SessionServer::generateSessionKey(std::string salt) {
	
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
 
int SessionServer::generateSessionId(std::string userId) {
  
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
 
int SessionServer::checkClientMachine(MachineClientServer MachineClient) {
	return 0;
}
 
int SessionServer::recordSessionServer(std::string idmachine, std::string iduser) {
  
 std::string sqlInsert = 
 //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, sessionKey, state, closepolicy, timeout) values ");
 std::string("insert into vsession\
 (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, \
 creation, sessionKey, state, closepolicy, timeout) values "); 
 //std::string("insert into vsession (vsessionid, clmachine_numclmachineid, users_numuserid, lastconnect, creation, closure, sessionKey, state, closepolicy, timeout) values ");
 
 std::string values = std::string("('" +msession.getSessionId()+"',"+idmachine+","+iduser+",\
 CURRENT_TIMESTAMP, CURRENT_TIMESTAMP, '"+msession.getSessionKey()+"',");
   
 values.append(convertToString(msession.getState())+",");
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
//int flagSessionId = 0 si la fonction utilise sessionKey et 1 si sessionIds 
bool SessionServer::exist(bool flagSessionId) {
 
  if (flagSessionId) {
  
      try {  
	  return (getState(true) != -1);  
      } catch (SystemException& e) {
	  throw e;
      } 
  }
  else {
      try {  
	  return (getState(true) != -1);  
      } catch (SystemException& e) {
	throw e;  
      }
  }
} 

//TODO utiliser cette fonction dans getState et getSessionKey ou simplement la généraliser pour
//éviter les répétitions de code
std::string SessionServer::getAttribut(std::string condition, std::string attrname) {
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

int SessionServer::getState(bool flagSessionId) {
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;
 std::string sqlCommand;
 
 if (flagSessionId) {
     sqlCommand = "SELECT state FROM vsession where vsessionid='"+msession.getSessionId()+"'";
 }
 else {
     sqlCommand = "SELECT state FROM vsession where sessionkey='"+msession.getSessionKey()+"'";
 }
 
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 try {
 result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } catch (SystemException& e) {
 throw e;
 }

 if (result->getNbTuples() != 0) {
      // res = true;
      result->print();
      std::vector<std::string> tmp = result->get(0);
  
  ii=tmp.begin();
  std::istringstream str(*ii);
  int value;
  str>>value;
  std::cout << "Value: "<< value;
  return value;
    
 } // if  if (result->getNbTuples() != 0)
 else {
  return -1;
 }
}
 
int SessionServer::getSessionkey(std::string idmachine, std::string iduser, bool flagAdmin) {
 
 DatabaseResult* result;
 std::vector<std::string>::iterator ii;

 std::cout <<"SESSION:"<< msession.getSessionId() <<std::endl;
 std::string sqlCommand;
 
 if (!flagAdmin) {
 sqlCommand = "SELECT sessionkey FROM vsession \
 where vsessionid='"+msession.getSessionId()+"' and clmachine_numclmachineid="+idmachine+" and users_numuserid="+iduser;
 }
 else {
 sqlCommand = "SELECT sessionkey FROM vsession where vsessionid='"+msession.getSessionId()+"'";  
}
 
 std::cout <<"SQL COMMAND:"<<sqlCommand;
 
 try {
 result = mdatabaseVishnu->getResult(sqlCommand.c_str());
 } catch (SystemException& e) {
 throw e;
 }

 if (result->getNbTuples() != 0) {
   std::cout << "Je suis là"; 
   // res = true;
      result->print();
      std::vector<std::string> tmp = result->get(0);
  
  ii=tmp.begin();
  //std::istringstream str(*ii);
  /*int value;
  str>>value;
  std::cout << "Value: "<< value;*/
   msession.setSessionKey(*ii);
  return 0;  
 } // if  if (result->getNbTuples() != 0)
 else {
  result->printAttributesNames(); 
  return -1;
 }

}



int 
SessionServer::solveConnectionMode(UMS_Data::ConnectOptions* connectOpt, UserServer user) {
  
  OptionValueServer optionValueServer;
  
  switch (connectOpt->getClosePolicy()) {
    case 0: 
      msession.setClosePolicy(optionValueServer.getClosureInfo(user.getAttribut("\
			            where userid='"+user.getData().getUserId()+"'")));
      //If the policy is not 2 (CLOSE_ON_DISCONNECT)
      if (msession.getClosePolicy() != 2) { 
	msession.setTimeout(optionValueServer.getClosureInfo(user.getAttribut("\
			    where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT"));
      } 
    break;

    case 1: 
      msession.setClosePolicy(1);
      if (connectOpt->getSessionInactivityDelay() != 0) {
	  msession.setTimeout(connectOpt->getSessionInactivityDelay());
      } //END the timeout is defined
      else {
	  msession.setTimeout(optionValueServer.getClosureInfo(user.getAttribut("\
			      where userid='"+user.getData().getUserId()+"'"), "VISHNU_TIMEOUT"));
      }// END ELSE the timeout is defined
    break;
    
    case 2: 
      msession.setClosePolicy(2); 
    break;
  }  
}
  
/*UMS_Data::ListSessions  SessionServer::list(SessionServer session, UMS_Data::ListSessionOptions  options)
{
	return 0;
}*/
